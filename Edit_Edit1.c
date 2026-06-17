
/*==================[inclusions]=============================================*/

#include "inc/Edit_Edit1.h"
#include "inc/systemTimer.h"
#include "inc/streamIn.h"

/*==================[internal state — one edit at a time per instance]=======*/

static void*    ed_activeVar = 0;                       /* 0 = no edit active        */
static uint8_t  ed_type      = 0;                       /* EDIT_TYPE_* of active var */
static char     ed_mask[EDIT_Edit1_MAXLEN + 1];      /* mask in edit              */
static char     ed_buf [EDIT_Edit1_MAXLEN + 1];      /* ASCII being edited        */
static char     ed_view[EDIT_Edit1_MAXLEN + 1];      /* provider output / scratch */
static uint8_t  ed_len       = 0;                       /* mask length               */
static uint8_t  ed_cursor    = 0;                       /* current logical position  */

/*==================[mask helpers]===========================================*/

/* Position domain: 0 = fixed (. , / : space), 1 = digit (# 0), 2 = sign (- +). */
static uint8_t ed_posKind(char m)
{
    if (m == '#' || m == '0') return 1;
    if (m == '-' || m == '+') return 2;
    return 0;
}

/* Number of decimal digit positions (digits after a '.'/',' separator). */
static uint8_t ed_decimals(const char* mask, uint8_t len)
{
    uint8_t i, dec = 0, seen = 0;
    for (i = 0; i < len; i++)
    {
        if (mask[i] == '.' || mask[i] == ',') { seen = 1; continue; }
        if (seen && (mask[i] == '#' || mask[i] == '0')) dec++;
    }
    return dec;
}

static uint64_t ed_pow10(uint8_t n)
{
    uint64_t p = 1;
    while (n) { p *= 10; n--; }
    return p;
}

/* |value| * 10^dec as an integer, plus the sign flag. */
static uint64_t ed_scaledAbs(void* var, uint8_t type, uint8_t dec, uint8_t* neg)
{
    uint64_t scale = ed_pow10(dec);
    *neg = 0;
    switch (type)
    {
        case EDIT_TYPE_U8:    return (uint64_t)(*(uint8_t*)  var) * scale;
        case EDIT_TYPE_U16:   return (uint64_t)(*(uint16_t*) var) * scale;
        case EDIT_TYPE_U32:   return (uint64_t)(*(uint32_t*) var) * scale;
        case EDIT_TYPE_U64:   return            (*(uint64_t*) var) * scale;
        case EDIT_TYPE_I8:    { int32_t v = *(int8_t*)  var; if (v < 0) { *neg = 1; v = -v; } return (uint64_t)v * scale; }
        case EDIT_TYPE_I16:   { int32_t v = *(int16_t*) var; if (v < 0) { *neg = 1; v = -v; } return (uint64_t)v * scale; }
        case EDIT_TYPE_I32:   { int64_t v = *(int32_t*) var; if (v < 0) { *neg = 1; v = -v; } return (uint64_t)v * scale; }
        case EDIT_TYPE_I64:   { int64_t v = *(int64_t*) var; if (v < 0) { *neg = 1; v = -v; } return (uint64_t)v * scale; }
        case EDIT_TYPE_FLOAT: { float   f = *(float*)   var; if (f < 0) { *neg = 1; f = -f; } return (uint64_t)(f * (float)scale + 0.5f); }
        /* EDIT_TYPE_CHAR (alphanumeric) -> v2 */
        default: return 0;
    }
}

/* Format the value into out[] following the picture mask (fixed width). */
static void ed_formatValue(void* var, uint8_t type, const char* mask, char* out)
{
    uint8_t  len = 0, dec, neg = 0, digitsEmitted = 0;
    uint64_t val;
    int16_t  i;

    while (mask[len] && len < EDIT_Edit1_MAXLEN) len++;
    dec = ed_decimals(mask, len);
    val = ed_scaledAbs(var, type, dec, &neg);

    for (i = (int16_t)len - 1; i >= 0; i--)
    {
        char    m = mask[i];
        uint8_t k = ed_posKind(m);
        if (k == 1)
        {
            uint8_t  digit = (uint8_t)(val % 10);
            uint64_t had   = val;
            val /= 10;
            if (digitsEmitted < dec)                         /* decimal: always a digit */
                out[i] = (char)('0' + digit);
            else if (had > 0 || digitsEmitted == dec)        /* integer significant or units */
                out[i] = (char)('0' + digit);
            else                                             /* leading: pad */
                out[i] = (m == '0') ? '0' : ' ';
            digitsEmitted++;
        }
        else if (k == 2)
        {
            out[i] = neg ? '-' : ((m == '+') ? '+' : ' ');
        }
        else
        {
            out[i] = m;                                      /* fixed */
        }
    }
    out[len] = 0;
}

/* Cursor navigation (skip fixed positions). */
static uint8_t ed_moveNext(uint8_t from)
{
    uint8_t c = from;
    while ((uint8_t)(c + 1) < ed_len) { c++; if (ed_posKind(ed_mask[c]) != 0) return c; }
    return from;
}
static uint8_t ed_movePrev(uint8_t from)
{
    uint8_t c = from;
    while (c > 0) { c--; if (ed_posKind(ed_mask[c]) != 0) return c; }
    return from;
}
static uint8_t ed_firstEditable(void)
{
    uint8_t i;
    for (i = 0; i < ed_len; i++) if (ed_posKind(ed_mask[i]) != 0) return i;
    return 0;
}

/* Copy buf -> view and, on the blink "off" phase, hide the cursor character. */
static void ed_applyBlink(void)
{
    uint8_t i;
    for (i = 0; i < ed_len; i++) ed_view[i] = ed_buf[i];
    ed_view[ed_len] = 0;
    if (((getSystemMilis() / EDIT_Edit1_BLINK_MS) & 1) != 0)
        ed_view[ed_cursor] = ' ';
}

/*==================[init]===================================================*/

void Edit_Edit1_init(void)
{
    ed_activeVar = 0;
    ed_len       = 0;
    ed_cursor    = 0;
}

/*==================[provider — display side]================================*/

char* Edit_Edit1_editableVar(void* var, uint8_t type, char* mask)
{
    if (ed_activeVar != 0 && var == ed_activeVar)
    {
        ed_applyBlink();              /* editing this var: buffer + blinking cursor */
        return ed_view;
    }
    ed_formatValue(var, type, mask, ed_view);   /* not editing: formatted value */
    return ed_view;
}

/*==================[edit control]===========================================*/

void Edit_Edit1_begin(void* var, uint8_t type, char* mask)
{
    uint8_t i;
    ed_activeVar = var;               /* auto-cancels any previous edit */
    ed_type      = type;
    ed_len       = 0;
    while (mask[ed_len] && ed_len < EDIT_Edit1_MAXLEN)
    {
        ed_mask[ed_len] = mask[ed_len];
        ed_len++;
    }
    ed_mask[ed_len] = 0;
    ed_formatValue(var, type, ed_mask, ed_buf);
    for (i = 0; i < ed_len; i++)      /* on entering edit: digit space -> '0'; positive sign space -> '+' */
    {
        if      (ed_posKind(ed_mask[i]) == 1 && ed_buf[i] == ' ') ed_buf[i] = '0';
        else if (ed_posKind(ed_mask[i]) == 2 && ed_buf[i] == ' ') ed_buf[i] = '+';
    }
    ed_cursor = ed_firstEditable();
}

static const char* ed_inPtr;
static uint16_t     ed_inLeft;
static char     ed_inGet(void)   { if (ed_inLeft) { ed_inLeft--; return *ed_inPtr++; } return 0; }
static uint16_t ed_inCount(void) { return ed_inLeft; }
static streamIn_t ed_inStream = { ed_inGet, ed_inCount };

static void ed_commit(void)
{
    uint8_t i, j = 0;
    if (ed_activeVar == 0) return;

    for (i = 0; i < ed_len; i++)      /* clean buffer: ',' -> '.', drop padding spaces and the '+' sign */
    {
        char c = ed_buf[i];
        if (c == ',') c = '.';
        if (c == ' ' || c == '+') continue;
        ed_view[j++] = c;
    }
    ed_view[j] = 0;
    ed_inPtr  = ed_view;
    ed_inLeft = j;

    switch (ed_type)
    {
        case EDIT_TYPE_U8:    *(uint8_t*)  ed_activeVar = streamIn_t_ptr_to_uint8_t(&ed_inStream);  break;
        case EDIT_TYPE_U16:   *(uint16_t*) ed_activeVar = streamIn_t_ptr_to_uint16_t(&ed_inStream); break;
        case EDIT_TYPE_U32:   *(uint32_t*) ed_activeVar = streamIn_t_ptr_to_uint32_t(&ed_inStream); break;
        case EDIT_TYPE_U64:   *(uint64_t*) ed_activeVar = streamIn_t_ptr_to_uint64_t(&ed_inStream); break;
        case EDIT_TYPE_I8:    *(int8_t*)   ed_activeVar = streamIn_t_ptr_to_int8_t(&ed_inStream);   break;
        case EDIT_TYPE_I16:   *(int16_t*)  ed_activeVar = streamIn_t_ptr_to_int16_t(&ed_inStream);  break;
        case EDIT_TYPE_I32:   *(int32_t*)  ed_activeVar = streamIn_t_ptr_to_int32_t(&ed_inStream);  break;
        case EDIT_TYPE_I64:   *(int64_t*)  ed_activeVar = streamIn_t_ptr_to_int64_t(&ed_inStream);  break;
        case EDIT_TYPE_FLOAT: *(float*)    ed_activeVar = streamIn_t_ptr_to_float(&ed_inStream);    break;
        /* EDIT_TYPE_CHAR (alphanumeric) -> v2 */
        default: break;
    }
    ed_activeVar = 0;                 /* close edit */
}

static void ed_abort(void)
{
    ed_activeVar = 0;                 /* discard, keep previous value */
}

void Edit_Edit1_accept(void) { ed_commit(); }


/*==================[navigation / editing]===================================*/

void Edit_Edit1_nextDigit(void)
{
    uint8_t next;
    if (ed_activeVar == 0) return;
    next = ed_moveNext(ed_cursor);
    if (next == ed_cursor) ed_commit();     /* past the least-significant digit -> accept */
    else                   ed_cursor = next;
}

void Edit_Edit1_prevDigit(void)
{
    uint8_t prev;
    if (ed_activeVar == 0) return;
    prev = ed_movePrev(ed_cursor);
    if (prev == ed_cursor) ed_abort();      /* before the most-significant pos/sign -> cancel */
    else                   ed_cursor = prev;
}

void Edit_Edit1_incDigit(void)
{
    char m, d;
    if (ed_activeVar == 0) return;
    m = ed_mask[ed_cursor];
    if (ed_posKind(m) == 1)
    {
        d = ed_buf[ed_cursor];
        if (d < '0' || d > '9') d = '0';
        ed_buf[ed_cursor] = (d == '9') ? '0' : (char)(d + 1);   /* wrap local */
    }
    else if (ed_posKind(m) == 2)
    {
        ed_buf[ed_cursor] = (ed_buf[ed_cursor] == '-') ? '+' : '-';   /* en edicion el signo alterna -/+ */
    }
}

void Edit_Edit1_decDigit(void)
{
    char m, d;
    if (ed_activeVar == 0) return;
    m = ed_mask[ed_cursor];
    if (ed_posKind(m) == 1)
    {
        d = ed_buf[ed_cursor];
        if (d < '0' || d > '9') d = '0';
        ed_buf[ed_cursor] = (d == '0') ? '9' : (char)(d - 1);   /* wrap local */
    }
    else if (ed_posKind(m) == 2)
    {
        ed_buf[ed_cursor] = (ed_buf[ed_cursor] == '-') ? '+' : '-';   /* en edicion el signo alterna -/+ */
    }
}

void Edit_Edit1_changeCharacter(char c)
{
    char m;
    if (ed_activeVar == 0) return;
    m = ed_mask[ed_cursor];
    if (ed_posKind(m) == 1)
    {
        if (c >= '0' && c <= '9') { ed_buf[ed_cursor] = c; ed_cursor = ed_moveNext(ed_cursor); }
    }
    else if (ed_posKind(m) == 2)
    {
        if (c == '-' || c == '+' || c == ' ') { ed_buf[ed_cursor] = (c == '-') ? '-' : '+'; ed_cursor = ed_moveNext(ed_cursor); }
    }
}

/*==================[end of file]============================================*/

