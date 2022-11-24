#ifndef _INSTR_OPND_API_H_
#define _INSTR_OPND_API_H_ 1

#include "codec.h"
#define AARCH64
#define X64
#define AARCHXX

#define TESTALL(mask, var) (((mask) & (var)) == (mask))
#define TESTANY(mask, var) (((mask) & (var)) != 0)
#define TEST TESTANY

#ifndef IN
#    define IN /* marks input param */
#endif
#ifndef OUT
#    define OUT /* marks output param */
#endif
#ifndef INOUT
#    define INOUT /* marks input+output param */
#endif

#ifndef INSTR_INLINE
#    ifdef DR_FAST_IR
#        define INSTR_INLINE inline
#    else
#        define INSTR_INLINE
#    endif
#endif

/* macros to make conditional compilation look prettier */
#ifdef DEBUG
#    define IF_DEBUG(x) x
#    define _IF_DEBUG(x) , x
#    define IF_DEBUG_(x) x,
#    define IF_DEBUG_ELSE(x, y) x
#    define IF_DEBUG_ELSE_0(x) (x)
#    define IF_DEBUG_ELSE_NULL(x) (x)
#else
#    define IF_DEBUG(x)
#    define _IF_DEBUG(x)
#    define IF_DEBUG_(x)
#    define IF_DEBUG_ELSE(x, y) y
#    define IF_DEBUG_ELSE_0(x) 0
#    define IF_DEBUG_ELSE_NULL(x) (NULL)
#endif

#ifdef DEBUG
#    define CLIENT_ASSERT(x, msg) apicheck(x, msg)
#else
#    define CLIENT_ASSERT(x, msg) /* PR 215261: nothing in release builds */
#    define CLIENT_ASSERT_(cond, msg)
#endif

#define CLIENT_ASSERT_BITFIELD_TRUNCATE(width, val, msg) \
    CLIENT_ASSERT((val) < (1 << ((width) + 1)), msg);

#define CLIENT_ASSERT_TRUNCATE(var, type, val, msg) \
    CLIENT_ASSERT(sizeof(type) == sizeof(var) && CHECK_TRUNCATE_TYPE_##type(val), msg)

#ifdef X86
#    define IF_X86(x) x
#    define IF_X86_ELSE(x, y) x
#    define IF_X86_(x) x,
#    define _IF_X86(x) , x
#    define IF_NOT_X86(x)
#    define IF_NOT_X86_(x)
#    define _IF_NOT_X86(x)
#else
#    define IF_X86(x)
#    define IF_X86_ELSE(x, y) y
#    define IF_X86_(x)
#    define _IF_X86(x)
#    define IF_NOT_X86(x) x
#    define IF_NOT_X86_(x) x,
#    define _IF_NOT_X86(x) , x
#endif

#ifdef ARM
#    define IF_ARM(x) x
#    define IF_ARM_ELSE(x, y) x
#    define IF_ARM_(x) x,
#    define _IF_ARM(x) , x
#    define IF_NOT_ARM(x)
#    define _IF_NOT_ARM(x)
#else
#    define IF_ARM(x)
#    define IF_ARM_ELSE(x, y) y
#    define IF_ARM_(x)
#    define _IF_ARM(x)
#    define IF_NOT_ARM(x) x
#    define _IF_NOT_ARM(x) , x
#endif

#ifdef AARCH64
#    define IF_AARCH64(x) x
#    define IF_AARCH64_ELSE(x, y) x
#    define IF_AARCH64_(x) x,
#    define _IF_AARCH64(x) , x
#    define IF_NOT_AARCH64(x)
#    define _IF_NOT_AARCH64(x)
#else
#    define IF_AARCH64(x)
#    define IF_AARCH64_ELSE(x, y) y
#    define IF_AARCH64_(x)
#    define _IF_AARCH64(x)
#    define IF_NOT_AARCH64(x) x
#    define _IF_NOT_AARCH64(x) , x
#endif

#ifdef AARCHXX
#    define IF_AARCHXX(x) x
#    define IF_AARCHXX_ELSE(x, y) x
#    define IF_AARCHXX_(x) x,
#    define _IF_AARCHXX(x) , x
#    define IF_NOT_AARCHXX(x)
#    define _IF_NOT_AARCHXX(x)
#else
#    define IF_AARCHXX(x)
#    define IF_AARCHXX_ELSE(x, y) y
#    define IF_AARCHXX_(x)
#    define _IF_AARCHXX(x)
#    define IF_NOT_AARCHXX(x) x
#    define _IF_NOT_AARCHXX(x) , x
#endif

#ifdef ANDROID
#    define IF_ANDROID(x) x
#    define IF_ANDROID_ELSE(x, y) x
#    define IF_NOT_ANDROID(x)
#else
#    define IF_ANDROID(x)
#    define IF_ANDROID_ELSE(x, y) y
#    define IF_NOT_ANDROID(x) x
#endif

#ifdef X64
#    define IF_X64(x) x
#    define IF_X64_ELSE(x, y) x
#    define IF_X64_(x) x,
#    define _IF_X64(x) , x
#    define IF_NOT_X64(x)
#    define _IF_NOT_X64(x)
#else
#    define IF_X64(x)
#    define IF_X64_ELSE(x, y) y
#    define IF_X64_(x)
#    define _IF_X64(x)
#    define IF_NOT_X64(x) x
#    define _IF_NOT_X64(x) , x
#endif

#if defined(X86) && !defined(X64)
#    define IF_X86_32(x) x
#else
#    define IF_X86_32(x)
#endif

#if defined(X86) && defined(X64)
#    define IF_X86_64(x) x
#    define IF_X86_64_ELSE(x, y) x
#    define IF_X86_64_(x) x,
#    define _IF_X86_64(x) , x
#    define IF_NOT_X86_64(x)
#    define _IF_NOT_X86_64(x)
#else
#    define IF_X86_64(x)
#    define IF_X86_64_ELSE(x, y) y
#    define IF_X86_64_(x)
#    define _IF_X86_64(x)
#    define IF_NOT_X86_64(x) x
#    define _IF_NOT_X86_64(x) , x
#endif

#if defined(X64) || defined(ARM)
#    define IF_X64_OR_ARM(x) x
#    define IF_NOT_X64_OR_ARM(x)
#else
#    define IF_X64_OR_ARM(x)
#    define IF_NOT_X64_OR_ARM(x) x
#endif

#ifdef WINDOWS
#    define DR_EXPORT __declspec(dllexport)
#    define LINK_ONCE __declspec(selectany)
#    define ALIGN_VAR(x) __declspec(align(x))
#    define INLINE_FORCED __forceinline
#    define WEAK /* no equivalent, but .obj overrides .lib */
#    define NOINLINE __declspec(noinline)
#else
/* We assume gcc is being used.  If the client is using -fvisibility
 * (in gcc >= 3.4) to not export symbols by default, setting
 * USE_VISIBILITY_ATTRIBUTES will properly export.
 */
#    ifdef USE_VISIBILITY_ATTRIBUTES
#        define DR_EXPORT __attribute__((visibility("default")))
#    else
#        define DR_EXPORT
#    endif
#    define LINK_ONCE __attribute__((weak))
#    define ALIGN_VAR(x) __attribute__((aligned(x)))
#    define INLINE_FORCED inline
#    define WEAK __attribute__((weak))
#    define NOINLINE __attribute__((noinline))
#endif

/* currently we always export statistics structure */
#define DYNAMORIO_STATS_EXPORTS 1

#ifdef WINDOWS
#    define DYNAMORIO_EXPORT __declspec(dllexport)
#elif defined(USE_VISIBILITY_ATTRIBUTES)
/* PR 262804: we use "protected" instead of "default" to ensure our
 * own uses won't be preempted.  Note that for DR_APP_API in
 * lib/dr_app.h we get a link error trying to use "protected": but we
 * don't use dr_app_* internally anyway, so leaving as default.
 */
#    define DYNAMORIO_EXPORT __attribute__((visibility("protected")))
#else
/* visibility attribute not available in gcc < 3.3 (we use linker script) */
#    define DYNAMORIO_EXPORT
#endif

/* We always export nowadays. */
#define DR_API DYNAMORIO_EXPORT
#if (defined(DEBUG) && defined(BUILD_TESTS)) || defined(UNSUPPORTED_API)
#    define DR_UNS_EXCEPT_TESTS_API DR_API
#else
#    define DR_UNS_EXCEPT_TESTS_API /* nothing */
#endif
#ifdef UNSUPPORTED_API
/* TODO i#4045: Remove unsupported API support.  After i#3092's header refactoring,
 * we can't just change a define and export these anymore anyway: they would have
 * to be moved to the _api.h public headers.
 */
#    define DR_UNS_API DR_API
#else
#    define DR_UNS_API /* nothing */
#endif

#ifndef DR_DO_NOT_DEFINE_uint
typedef unsigned int uint;
#endif
#ifndef DR_DO_NOT_DEFINE_ushort
typedef unsigned short ushort;
#endif
#ifndef DR_DO_NOT_DEFINE_byte
typedef unsigned char byte;
#endif
#ifndef DR_DO_NOT_DEFINE_sbyte
typedef signed char sbyte;
#endif

#ifndef DR_DEFINE_FOR_uint64
	#define DR_DEFINE_FOR_uint64 unsigned long int
#endif
#ifndef DR_DEFINE_FOR_int64
	#define DR_DEFINE_FOR_int64 long int
#endif

#ifndef DR_DO_NOT_DEFINE_int64
typedef DR_DEFINE_FOR_int64 int64;
#endif
#ifndef DR_DO_NOT_DEFINE_uint64
typedef DR_DEFINE_FOR_uint64 uint64;
#endif
typedef byte *app_pc;

/* a register value: could be of any type; size is what matters. */
#ifdef X64
typedef uint64 reg_t;
#else
typedef uint reg_t;
#endif
/* integer whose size is based on pointers: ptr diff, mask, etc. */
typedef reg_t ptr_uint_t;
#ifdef X64
typedef int64 ptr_int_t;
#else
typedef int ptr_int_t;

#endif
/* We assume all addressing regs are in the lower 256 of the DR_REG_ enum. */
#    define REG_SPECIFIER_BITS 8
#    define SCALE_SPECIFIER_BITS 4

typedef ptr_int_t atomic_int_t;

/* we avoid typedef-ing the enum, as its storage size is compiler-specific */
typedef ushort reg_id_t; /**< The type of a DR_REG_ enum value. */
/* For x86 we do store reg_id_t here, but the x86 DR_REG_ enum is small enough
 * (checked in d_r_arch_init()).
 */
typedef byte opnd_size_t; /**< The type of an OPSZ_ enum value. */

enum {
    INSTR_DO_NOT_MANGLE = 0x00200000,
};

/****************************************************************************
 * OPCODES
 */
/**
 * @file dr_ir_opcodes_aarch64.h
 * @brief Instruction opcode constants for AArch64.
 */
/** Opcode constants for use in the instr_t data structure. */
enum {
/*   0 */     OP_INVALID,  /* NULL, */ /**< INVALID opcode */
/*   1 */     OP_UNDECODED,  /* NULL, */ /**< UNDECODED opcode */
/*   2 */     OP_CONTD,    /* NULL, */ /**< CONTD opcode */
/*   3 */     OP_LABEL,    /* NULL, */ /**< LABEL opcode */
/*   4 */    OP_xx, /* placeholder for undecoded instructions */
/*   5 */    OP_ldstex, /* single-entry single-exit block with exclusive load/store */

/*    6 */     OP_abs = 6, /**< AArch64 abs opcode. */
/*    7 */     OP_adc = 7, /**< AArch64 adc opcode. */
/*    8 */     OP_adcs = 8, /**< AArch64 adcs opcode. */
/*    9 */     OP_add = 9, /**< AArch64 add opcode. */
/*   10 */     OP_addhn = 10, /**< AArch64 addhn opcode. */
/*   11 */     OP_addhn2 = 11, /**< AArch64 addhn2 opcode. */
/*   12 */     OP_addp = 12, /**< AArch64 addp opcode. */
/*   13 */     OP_adds = 13, /**< AArch64 adds opcode. */
/*   14 */     OP_addv = 14, /**< AArch64 addv opcode. */
/*   15 */     OP_adr = 15, /**< AArch64 adr opcode. */
/*   16 */     OP_adrp = 16, /**< AArch64 adrp opcode. */
/*   17 */     OP_aesd = 17, /**< AArch64 aesd opcode. */
/*   18 */     OP_aese = 18, /**< AArch64 aese opcode. */
/*   19 */     OP_aesimc = 19, /**< AArch64 aesimc opcode. */
/*   20 */     OP_aesmc = 20, /**< AArch64 aesmc opcode. */
/*   21 */     OP_and = 21, /**< AArch64 and opcode. */
/*   22 */     OP_ands = 22, /**< AArch64 ands opcode. */
/*   23 */     OP_asrv = 23, /**< AArch64 asrv opcode. */
/*   24 */     OP_autia1716 = 24, /**< AArch64 autia1716 opcode. */
/*   25 */     OP_autib1716 = 25, /**< AArch64 autib1716 opcode. */
/*   26 */     OP_b = 26, /**< AArch64 b opcode. */
/*   27 */     OP_bcond = 27, /**< AArch64 bcond opcode. */
/*   28 */     OP_bfm = 28, /**< AArch64 bfm opcode. */
/*   29 */     OP_bic = 29, /**< AArch64 bic opcode. */
/*   30 */     OP_bics = 30, /**< AArch64 bics opcode. */
/*   31 */     OP_bif = 31, /**< AArch64 bif opcode. */
/*   32 */     OP_bit = 32, /**< AArch64 bit opcode. */
/*   33 */     OP_bl = 33, /**< AArch64 bl opcode. */
/*   34 */     OP_blr = 34, /**< AArch64 blr opcode. */
/*   35 */     OP_br = 35, /**< AArch64 br opcode. */
/*   36 */     OP_brk = 36, /**< AArch64 brk opcode. */
/*   37 */     OP_bsl = 37, /**< AArch64 bsl opcode. */
/*   38 */     OP_cas = 38, /**< AArch64 cas opcode. */
/*   39 */     OP_casa = 39, /**< AArch64 casa opcode. */
/*   40 */     OP_casab = 40, /**< AArch64 casab opcode. */
/*   41 */     OP_casah = 41, /**< AArch64 casah opcode. */
/*   42 */     OP_casal = 42, /**< AArch64 casal opcode. */
/*   43 */     OP_casalb = 43, /**< AArch64 casalb opcode. */
/*   44 */     OP_casalh = 44, /**< AArch64 casalh opcode. */
/*   45 */     OP_casb = 45, /**< AArch64 casb opcode. */
/*   46 */     OP_cash = 46, /**< AArch64 cash opcode. */
/*   47 */     OP_casl = 47, /**< AArch64 casl opcode. */
/*   48 */     OP_caslb = 48, /**< AArch64 caslb opcode. */
/*   49 */     OP_caslh = 49, /**< AArch64 caslh opcode. */
/*   50 */     OP_casp = 50, /**< AArch64 casp opcode. */
/*   51 */     OP_caspa = 51, /**< AArch64 caspa opcode. */
/*   52 */     OP_caspal = 52, /**< AArch64 caspal opcode. */
/*   53 */     OP_caspl = 53, /**< AArch64 caspl opcode. */
/*   54 */     OP_cbnz = 54, /**< AArch64 cbnz opcode. */
/*   55 */     OP_cbz = 55, /**< AArch64 cbz opcode. */
/*   56 */     OP_ccmn = 56, /**< AArch64 ccmn opcode. */
/*   57 */     OP_ccmp = 57, /**< AArch64 ccmp opcode. */
/*   58 */     OP_clrex = 58, /**< AArch64 clrex opcode. */
/*   59 */     OP_cls = 59, /**< AArch64 cls opcode. */
/*   60 */     OP_clz = 60, /**< AArch64 clz opcode. */
/*   61 */     OP_cmeq = 61, /**< AArch64 cmeq opcode. */
/*   62 */     OP_cmge = 62, /**< AArch64 cmge opcode. */
/*   63 */     OP_cmgt = 63, /**< AArch64 cmgt opcode. */
/*   64 */     OP_cmhi = 64, /**< AArch64 cmhi opcode. */
/*   65 */     OP_cmhs = 65, /**< AArch64 cmhs opcode. */
/*   66 */     OP_cmle = 66, /**< AArch64 cmle opcode. */
/*   67 */     OP_cmlt = 67, /**< AArch64 cmlt opcode. */
/*   68 */     OP_cmtst = 68, /**< AArch64 cmtst opcode. */
/*   69 */     OP_cnt = 69, /**< AArch64 cnt opcode. */
/*   70 */     OP_crc32b = 70, /**< AArch64 crc32b opcode. */
/*   71 */     OP_crc32cb = 71, /**< AArch64 crc32cb opcode. */
/*   72 */     OP_crc32ch = 72, /**< AArch64 crc32ch opcode. */
/*   73 */     OP_crc32cw = 73, /**< AArch64 crc32cw opcode. */
/*   74 */     OP_crc32cx = 74, /**< AArch64 crc32cx opcode. */
/*   75 */     OP_crc32h = 75, /**< AArch64 crc32h opcode. */
/*   76 */     OP_crc32w = 76, /**< AArch64 crc32w opcode. */
/*   77 */     OP_crc32x = 77, /**< AArch64 crc32x opcode. */
/*   78 */     OP_csel = 78, /**< AArch64 csel opcode. */
/*   79 */     OP_csinc = 79, /**< AArch64 csinc opcode. */
/*   80 */     OP_csinv = 80, /**< AArch64 csinv opcode. */
/*   81 */     OP_csneg = 81, /**< AArch64 csneg opcode. */
/*   82 */     OP_dcps1 = 82, /**< AArch64 dcps1 opcode. */
/*   83 */     OP_dcps2 = 83, /**< AArch64 dcps2 opcode. */
/*   84 */     OP_dcps3 = 84, /**< AArch64 dcps3 opcode. */
/*   85 */     OP_dmb = 85, /**< AArch64 dmb opcode. */
/*   86 */     OP_drps = 86, /**< AArch64 drps opcode. */
/*   87 */     OP_dsb = 87, /**< AArch64 dsb opcode. */
/*   88 */     OP_dup = 88, /**< AArch64 dup opcode. */
/*   89 */     OP_eon = 89, /**< AArch64 eon opcode. */
/*   90 */     OP_eor = 90, /**< AArch64 eor opcode. */
/*   91 */     OP_eret = 91, /**< AArch64 eret opcode. */
/*   92 */     OP_ext = 92, /**< AArch64 ext opcode. */
/*   93 */     OP_extr = 93, /**< AArch64 extr opcode. */
/*   94 */     OP_fabd = 94, /**< AArch64 fabd opcode. */
/*   95 */     OP_fabs = 95, /**< AArch64 fabs opcode. */
/*   96 */     OP_facge = 96, /**< AArch64 facge opcode. */
/*   97 */     OP_facgt = 97, /**< AArch64 facgt opcode. */
/*   98 */     OP_fadd = 98, /**< AArch64 fadd opcode. */
/*   99 */     OP_faddp = 99, /**< AArch64 faddp opcode. */
/*  100 */     OP_fccmp = 100, /**< AArch64 fccmp opcode. */
/*  101 */     OP_fccmpe = 101, /**< AArch64 fccmpe opcode. */
/*  102 */     OP_fcmeq = 102, /**< AArch64 fcmeq opcode. */
/*  103 */     OP_fcmge = 103, /**< AArch64 fcmge opcode. */
/*  104 */     OP_fcmgt = 104, /**< AArch64 fcmgt opcode. */
/*  105 */     OP_fcmle = 105, /**< AArch64 fcmle opcode. */
/*  106 */     OP_fcmlt = 106, /**< AArch64 fcmlt opcode. */
/*  107 */     OP_fcmp = 107, /**< AArch64 fcmp opcode. */
/*  108 */     OP_fcmpe = 108, /**< AArch64 fcmpe opcode. */
/*  109 */     OP_fcsel = 109, /**< AArch64 fcsel opcode. */
/*  110 */     OP_fcvt = 110, /**< AArch64 fcvt opcode. */
/*  111 */     OP_fcvtas = 111, /**< AArch64 fcvtas opcode. */
/*  112 */     OP_fcvtau = 112, /**< AArch64 fcvtau opcode. */
/*  113 */     OP_fcvtl = 113, /**< AArch64 fcvtl opcode. */
/*  114 */     OP_fcvtl2 = 114, /**< AArch64 fcvtl2 opcode. */
/*  115 */     OP_fcvtms = 115, /**< AArch64 fcvtms opcode. */
/*  116 */     OP_fcvtmu = 116, /**< AArch64 fcvtmu opcode. */
/*  117 */     OP_fcvtn = 117, /**< AArch64 fcvtn opcode. */
/*  118 */     OP_fcvtn2 = 118, /**< AArch64 fcvtn2 opcode. */
/*  119 */     OP_fcvtns = 119, /**< AArch64 fcvtns opcode. */
/*  120 */     OP_fcvtnu = 120, /**< AArch64 fcvtnu opcode. */
/*  121 */     OP_fcvtps = 121, /**< AArch64 fcvtps opcode. */
/*  122 */     OP_fcvtpu = 122, /**< AArch64 fcvtpu opcode. */
/*  123 */     OP_fcvtxn = 123, /**< AArch64 fcvtxn opcode. */
/*  124 */     OP_fcvtxn2 = 124, /**< AArch64 fcvtxn2 opcode. */
/*  125 */     OP_fcvtzs = 125, /**< AArch64 fcvtzs opcode. */
/*  126 */     OP_fcvtzu = 126, /**< AArch64 fcvtzu opcode. */
/*  127 */     OP_fdiv = 127, /**< AArch64 fdiv opcode. */
/*  128 */     OP_fmadd = 128, /**< AArch64 fmadd opcode. */
/*  129 */     OP_fmax = 129, /**< AArch64 fmax opcode. */
/*  130 */     OP_fmaxnm = 130, /**< AArch64 fmaxnm opcode. */
/*  131 */     OP_fmaxnmp = 131, /**< AArch64 fmaxnmp opcode. */
/*  132 */     OP_fmaxnmv = 132, /**< AArch64 fmaxnmv opcode. */
/*  133 */     OP_fmaxp = 133, /**< AArch64 fmaxp opcode. */
/*  134 */     OP_fmaxv = 134, /**< AArch64 fmaxv opcode. */
/*  135 */     OP_fmin = 135, /**< AArch64 fmin opcode. */
/*  136 */     OP_fminnm = 136, /**< AArch64 fminnm opcode. */
/*  137 */     OP_fminnmp = 137, /**< AArch64 fminnmp opcode. */
/*  138 */     OP_fminnmv = 138, /**< AArch64 fminnmv opcode. */
/*  139 */     OP_fminp = 139, /**< AArch64 fminp opcode. */
/*  140 */     OP_fminv = 140, /**< AArch64 fminv opcode. */
/*  141 */     OP_fmla = 141, /**< AArch64 fmla opcode. */
/*  142 */     OP_fmlal = 142, /**< AArch64 fmlal opcode. */
/*  143 */     OP_fmlal2 = 143, /**< AArch64 fmlal2 opcode. */
/*  144 */     OP_fmls = 144, /**< AArch64 fmls opcode. */
/*  145 */     OP_fmlsl = 145, /**< AArch64 fmlsl opcode. */
/*  146 */     OP_fmlsl2 = 146, /**< AArch64 fmlsl2 opcode. */
/*  147 */     OP_fmov = 147, /**< AArch64 fmov opcode. */
/*  148 */     OP_fmsub = 148, /**< AArch64 fmsub opcode. */
/*  149 */     OP_fmul = 149, /**< AArch64 fmul opcode. */
/*  150 */     OP_fmulx = 150, /**< AArch64 fmulx opcode. */
/*  151 */     OP_fneg = 151, /**< AArch64 fneg opcode. */
/*  152 */     OP_fnmadd = 152, /**< AArch64 fnmadd opcode. */
/*  153 */     OP_fnmsub = 153, /**< AArch64 fnmsub opcode. */
/*  154 */     OP_fnmul = 154, /**< AArch64 fnmul opcode. */
/*  155 */     OP_frecpe = 155, /**< AArch64 frecpe opcode. */
/*  156 */     OP_frecps = 156, /**< AArch64 frecps opcode. */
/*  157 */     OP_frecpx = 157, /**< AArch64 frecpx opcode. */
/*  158 */     OP_frinta = 158, /**< AArch64 frinta opcode. */
/*  159 */     OP_frinti = 159, /**< AArch64 frinti opcode. */
/*  160 */     OP_frintm = 160, /**< AArch64 frintm opcode. */
/*  161 */     OP_frintn = 161, /**< AArch64 frintn opcode. */
/*  162 */     OP_frintp = 162, /**< AArch64 frintp opcode. */
/*  163 */     OP_frintx = 163, /**< AArch64 frintx opcode. */
/*  164 */     OP_frintz = 164, /**< AArch64 frintz opcode. */
/*  165 */     OP_frsqrte = 165, /**< AArch64 frsqrte opcode. */
/*  166 */     OP_frsqrts = 166, /**< AArch64 frsqrts opcode. */
/*  167 */     OP_fsqrt = 167, /**< AArch64 fsqrt opcode. */
/*  168 */     OP_fsub = 168, /**< AArch64 fsub opcode. */
/*  169 */     OP_hlt = 169, /**< AArch64 hlt opcode. */
/*  170 */     OP_hvc = 170, /**< AArch64 hvc opcode. */
/*  171 */     OP_ins = 171, /**< AArch64 ins opcode. */
/*  172 */     OP_isb = 172, /**< AArch64 isb opcode. */
/*  173 */     OP_ld1 = 173, /**< AArch64 ld1 opcode. */
/*  174 */     OP_ld1r = 174, /**< AArch64 ld1r opcode. */
/*  175 */     OP_ld2 = 175, /**< AArch64 ld2 opcode. */
/*  176 */     OP_ld2r = 176, /**< AArch64 ld2r opcode. */
/*  177 */     OP_ld3 = 177, /**< AArch64 ld3 opcode. */
/*  178 */     OP_ld3r = 178, /**< AArch64 ld3r opcode. */
/*  179 */     OP_ld4 = 179, /**< AArch64 ld4 opcode. */
/*  180 */     OP_ld4r = 180, /**< AArch64 ld4r opcode. */
/*  181 */     OP_ldadd = 181, /**< AArch64 ldadd opcode. */
/*  182 */     OP_ldadda = 182, /**< AArch64 ldadda opcode. */
/*  183 */     OP_ldaddab = 183, /**< AArch64 ldaddab opcode. */
/*  184 */     OP_ldaddah = 184, /**< AArch64 ldaddah opcode. */
/*  185 */     OP_ldaddal = 185, /**< AArch64 ldaddal opcode. */
/*  186 */     OP_ldaddalb = 186, /**< AArch64 ldaddalb opcode. */
/*  187 */     OP_ldaddalh = 187, /**< AArch64 ldaddalh opcode. */
/*  188 */     OP_ldaddb = 188, /**< AArch64 ldaddb opcode. */
/*  189 */     OP_ldaddh = 189, /**< AArch64 ldaddh opcode. */
/*  190 */     OP_ldaddl = 190, /**< AArch64 ldaddl opcode. */
/*  191 */     OP_ldaddlb = 191, /**< AArch64 ldaddlb opcode. */
/*  192 */     OP_ldaddlh = 192, /**< AArch64 ldaddlh opcode. */
/*  193 */     OP_ldar = 193, /**< AArch64 ldar opcode. */
/*  194 */     OP_ldarb = 194, /**< AArch64 ldarb opcode. */
/*  195 */     OP_ldarh = 195, /**< AArch64 ldarh opcode. */
/*  196 */     OP_ldaxp = 196, /**< AArch64 ldaxp opcode. */
/*  197 */     OP_ldaxr = 197, /**< AArch64 ldaxr opcode. */
/*  198 */     OP_ldaxrb = 198, /**< AArch64 ldaxrb opcode. */
/*  199 */     OP_ldaxrh = 199, /**< AArch64 ldaxrh opcode. */
/*  200 */     OP_ldclr = 200, /**< AArch64 ldclr opcode. */
/*  201 */     OP_ldclra = 201, /**< AArch64 ldclra opcode. */
/*  202 */     OP_ldclrab = 202, /**< AArch64 ldclrab opcode. */
/*  203 */     OP_ldclrah = 203, /**< AArch64 ldclrah opcode. */
/*  204 */     OP_ldclral = 204, /**< AArch64 ldclral opcode. */
/*  205 */     OP_ldclralb = 205, /**< AArch64 ldclralb opcode. */
/*  206 */     OP_ldclralh = 206, /**< AArch64 ldclralh opcode. */
/*  207 */     OP_ldclrb = 207, /**< AArch64 ldclrb opcode. */
/*  208 */     OP_ldclrh = 208, /**< AArch64 ldclrh opcode. */
/*  209 */     OP_ldclrl = 209, /**< AArch64 ldclrl opcode. */
/*  210 */     OP_ldclrlb = 210, /**< AArch64 ldclrlb opcode. */
/*  211 */     OP_ldclrlh = 211, /**< AArch64 ldclrlh opcode. */
/*  212 */     OP_ldeor = 212, /**< AArch64 ldeor opcode. */
/*  213 */     OP_ldeora = 213, /**< AArch64 ldeora opcode. */
/*  214 */     OP_ldeorab = 214, /**< AArch64 ldeorab opcode. */
/*  215 */     OP_ldeorah = 215, /**< AArch64 ldeorah opcode. */
/*  216 */     OP_ldeoral = 216, /**< AArch64 ldeoral opcode. */
/*  217 */     OP_ldeoralb = 217, /**< AArch64 ldeoralb opcode. */
/*  218 */     OP_ldeoralh = 218, /**< AArch64 ldeoralh opcode. */
/*  219 */     OP_ldeorb = 219, /**< AArch64 ldeorb opcode. */
/*  220 */     OP_ldeorh = 220, /**< AArch64 ldeorh opcode. */
/*  221 */     OP_ldeorl = 221, /**< AArch64 ldeorl opcode. */
/*  222 */     OP_ldeorlb = 222, /**< AArch64 ldeorlb opcode. */
/*  223 */     OP_ldeorlh = 223, /**< AArch64 ldeorlh opcode. */
/*  224 */     OP_ldnp = 224, /**< AArch64 ldnp opcode. */
/*  225 */     OP_ldp = 225, /**< AArch64 ldp opcode. */
/*  226 */     OP_ldpsw = 226, /**< AArch64 ldpsw opcode. */
/*  227 */     OP_ldr = 227, /**< AArch64 ldr opcode. */
/*  228 */     OP_ldrb = 228, /**< AArch64 ldrb opcode. */
/*  229 */     OP_ldrh = 229, /**< AArch64 ldrh opcode. */
/*  230 */     OP_ldrsb = 230, /**< AArch64 ldrsb opcode. */
/*  231 */     OP_ldrsh = 231, /**< AArch64 ldrsh opcode. */
/*  232 */     OP_ldrsw = 232, /**< AArch64 ldrsw opcode. */
/*  233 */     OP_ldset = 233, /**< AArch64 ldset opcode. */
/*  234 */     OP_ldseta = 234, /**< AArch64 ldseta opcode. */
/*  235 */     OP_ldsetab = 235, /**< AArch64 ldsetab opcode. */
/*  236 */     OP_ldsetah = 236, /**< AArch64 ldsetah opcode. */
/*  237 */     OP_ldsetal = 237, /**< AArch64 ldsetal opcode. */
/*  238 */     OP_ldsetalb = 238, /**< AArch64 ldsetalb opcode. */
/*  239 */     OP_ldsetalh = 239, /**< AArch64 ldsetalh opcode. */
/*  240 */     OP_ldsetb = 240, /**< AArch64 ldsetb opcode. */
/*  241 */     OP_ldseth = 241, /**< AArch64 ldseth opcode. */
/*  242 */     OP_ldsetl = 242, /**< AArch64 ldsetl opcode. */
/*  243 */     OP_ldsetlb = 243, /**< AArch64 ldsetlb opcode. */
/*  244 */     OP_ldsetlh = 244, /**< AArch64 ldsetlh opcode. */
/*  245 */     OP_ldsmax = 245, /**< AArch64 ldsmax opcode. */
/*  246 */     OP_ldsmaxa = 246, /**< AArch64 ldsmaxa opcode. */
/*  247 */     OP_ldsmaxab = 247, /**< AArch64 ldsmaxab opcode. */
/*  248 */     OP_ldsmaxah = 248, /**< AArch64 ldsmaxah opcode. */
/*  249 */     OP_ldsmaxal = 249, /**< AArch64 ldsmaxal opcode. */
/*  250 */     OP_ldsmaxalb = 250, /**< AArch64 ldsmaxalb opcode. */
/*  251 */     OP_ldsmaxalh = 251, /**< AArch64 ldsmaxalh opcode. */
/*  252 */     OP_ldsmaxb = 252, /**< AArch64 ldsmaxb opcode. */
/*  253 */     OP_ldsmaxh = 253, /**< AArch64 ldsmaxh opcode. */
/*  254 */     OP_ldsmaxl = 254, /**< AArch64 ldsmaxl opcode. */
/*  255 */     OP_ldsmaxlb = 255, /**< AArch64 ldsmaxlb opcode. */
/*  256 */     OP_ldsmaxlh = 256, /**< AArch64 ldsmaxlh opcode. */
/*  257 */     OP_ldsmin = 257, /**< AArch64 ldsmin opcode. */
/*  258 */     OP_ldsmina = 258, /**< AArch64 ldsmina opcode. */
/*  259 */     OP_ldsminab = 259, /**< AArch64 ldsminab opcode. */
/*  260 */     OP_ldsminah = 260, /**< AArch64 ldsminah opcode. */
/*  261 */     OP_ldsminal = 261, /**< AArch64 ldsminal opcode. */
/*  262 */     OP_ldsminalb = 262, /**< AArch64 ldsminalb opcode. */
/*  263 */     OP_ldsminalh = 263, /**< AArch64 ldsminalh opcode. */
/*  264 */     OP_ldsminb = 264, /**< AArch64 ldsminb opcode. */
/*  265 */     OP_ldsminh = 265, /**< AArch64 ldsminh opcode. */
/*  266 */     OP_ldsminl = 266, /**< AArch64 ldsminl opcode. */
/*  267 */     OP_ldsminlb = 267, /**< AArch64 ldsminlb opcode. */
/*  268 */     OP_ldsminlh = 268, /**< AArch64 ldsminlh opcode. */
/*  269 */     OP_ldtr = 269, /**< AArch64 ldtr opcode. */
/*  270 */     OP_ldtrb = 270, /**< AArch64 ldtrb opcode. */
/*  271 */     OP_ldtrh = 271, /**< AArch64 ldtrh opcode. */
/*  272 */     OP_ldtrsb = 272, /**< AArch64 ldtrsb opcode. */
/*  273 */     OP_ldtrsh = 273, /**< AArch64 ldtrsh opcode. */
/*  274 */     OP_ldtrsw = 274, /**< AArch64 ldtrsw opcode. */
/*  275 */     OP_ldumax = 275, /**< AArch64 ldumax opcode. */
/*  276 */     OP_ldumaxa = 276, /**< AArch64 ldumaxa opcode. */
/*  277 */     OP_ldumaxab = 277, /**< AArch64 ldumaxab opcode. */
/*  278 */     OP_ldumaxah = 278, /**< AArch64 ldumaxah opcode. */
/*  279 */     OP_ldumaxal = 279, /**< AArch64 ldumaxal opcode. */
/*  280 */     OP_ldumaxalb = 280, /**< AArch64 ldumaxalb opcode. */
/*  281 */     OP_ldumaxalh = 281, /**< AArch64 ldumaxalh opcode. */
/*  282 */     OP_ldumaxb = 282, /**< AArch64 ldumaxb opcode. */
/*  283 */     OP_ldumaxh = 283, /**< AArch64 ldumaxh opcode. */
/*  284 */     OP_ldumaxl = 284, /**< AArch64 ldumaxl opcode. */
/*  285 */     OP_ldumaxlb = 285, /**< AArch64 ldumaxlb opcode. */
/*  286 */     OP_ldumaxlh = 286, /**< AArch64 ldumaxlh opcode. */
/*  287 */     OP_ldumin = 287, /**< AArch64 ldumin opcode. */
/*  288 */     OP_ldumina = 288, /**< AArch64 ldumina opcode. */
/*  289 */     OP_lduminab = 289, /**< AArch64 lduminab opcode. */
/*  290 */     OP_lduminah = 290, /**< AArch64 lduminah opcode. */
/*  291 */     OP_lduminal = 291, /**< AArch64 lduminal opcode. */
/*  292 */     OP_lduminalb = 292, /**< AArch64 lduminalb opcode. */
/*  293 */     OP_lduminalh = 293, /**< AArch64 lduminalh opcode. */
/*  294 */     OP_lduminb = 294, /**< AArch64 lduminb opcode. */
/*  295 */     OP_lduminh = 295, /**< AArch64 lduminh opcode. */
/*  296 */     OP_lduminl = 296, /**< AArch64 lduminl opcode. */
/*  297 */     OP_lduminlb = 297, /**< AArch64 lduminlb opcode. */
/*  298 */     OP_lduminlh = 298, /**< AArch64 lduminlh opcode. */
/*  299 */     OP_ldur = 299, /**< AArch64 ldur opcode. */
/*  300 */     OP_ldurb = 300, /**< AArch64 ldurb opcode. */
/*  301 */     OP_ldurh = 301, /**< AArch64 ldurh opcode. */
/*  302 */     OP_ldursb = 302, /**< AArch64 ldursb opcode. */
/*  303 */     OP_ldursh = 303, /**< AArch64 ldursh opcode. */
/*  304 */     OP_ldursw = 304, /**< AArch64 ldursw opcode. */
/*  305 */     OP_ldxp = 305, /**< AArch64 ldxp opcode. */
/*  306 */     OP_ldxr = 306, /**< AArch64 ldxr opcode. */
/*  307 */     OP_ldxrb = 307, /**< AArch64 ldxrb opcode. */
/*  308 */     OP_ldxrh = 308, /**< AArch64 ldxrh opcode. */
/*  309 */     OP_lslv = 309, /**< AArch64 lslv opcode. */
/*  310 */     OP_lsrv = 310, /**< AArch64 lsrv opcode. */
/*  311 */     OP_madd = 311, /**< AArch64 madd opcode. */
/*  312 */     OP_mla = 312, /**< AArch64 mla opcode. */
/*  313 */     OP_mls = 313, /**< AArch64 mls opcode. */
/*  314 */     OP_movi = 314, /**< AArch64 movi opcode. */
/*  315 */     OP_movk = 315, /**< AArch64 movk opcode. */
/*  316 */     OP_movn = 316, /**< AArch64 movn opcode. */
/*  317 */     OP_movz = 317, /**< AArch64 movz opcode. */
/*  318 */     OP_mrs = 318, /**< AArch64 mrs opcode. */
/*  319 */     OP_msr = 319, /**< AArch64 msr opcode. */
/*  320 */     OP_msub = 320, /**< AArch64 msub opcode. */
/*  321 */     OP_mul = 321, /**< AArch64 mul opcode. */
/*  322 */     OP_mvni = 322, /**< AArch64 mvni opcode. */
/*  323 */     OP_neg = 323, /**< AArch64 neg opcode. */
/*  324 */     OP_nop = 324, /**< AArch64 nop opcode. */
/*  325 */     OP_not = 325, /**< AArch64 not opcode. */
/*  326 */     OP_orn = 326, /**< AArch64 orn opcode. */
/*  327 */     OP_orr = 327, /**< AArch64 orr opcode. */
/*  328 */     OP_pmul = 328, /**< AArch64 pmul opcode. */
/*  329 */     OP_pmull = 329, /**< AArch64 pmull opcode. */
/*  330 */     OP_pmull2 = 330, /**< AArch64 pmull2 opcode. */
/*  331 */     OP_prfm = 331, /**< AArch64 prfm opcode. */
/*  332 */     OP_prfum = 332, /**< AArch64 prfum opcode. */
/*  333 */     OP_raddhn = 333, /**< AArch64 raddhn opcode. */
/*  334 */     OP_raddhn2 = 334, /**< AArch64 raddhn2 opcode. */
/*  335 */     OP_rbit = 335, /**< AArch64 rbit opcode. */
/*  336 */     OP_ret = 336, /**< AArch64 ret opcode. */
/*  337 */     OP_rev = 337, /**< AArch64 rev opcode. */
/*  338 */     OP_rev16 = 338, /**< AArch64 rev16 opcode. */
/*  339 */     OP_rev32 = 339, /**< AArch64 rev32 opcode. */
/*  340 */     OP_rev64 = 340, /**< AArch64 rev64 opcode. */
/*  341 */     OP_rorv = 341, /**< AArch64 rorv opcode. */
/*  342 */     OP_rshrn = 342, /**< AArch64 rshrn opcode. */
/*  343 */     OP_rshrn2 = 343, /**< AArch64 rshrn2 opcode. */
/*  344 */     OP_rsubhn = 344, /**< AArch64 rsubhn opcode. */
/*  345 */     OP_rsubhn2 = 345, /**< AArch64 rsubhn2 opcode. */
/*  346 */     OP_saba = 346, /**< AArch64 saba opcode. */
/*  347 */     OP_sabal = 347, /**< AArch64 sabal opcode. */
/*  348 */     OP_sabal2 = 348, /**< AArch64 sabal2 opcode. */
/*  349 */     OP_sabd = 349, /**< AArch64 sabd opcode. */
/*  350 */     OP_sabdl = 350, /**< AArch64 sabdl opcode. */
/*  351 */     OP_sabdl2 = 351, /**< AArch64 sabdl2 opcode. */
/*  352 */     OP_sadalp = 352, /**< AArch64 sadalp opcode. */
/*  353 */     OP_saddl = 353, /**< AArch64 saddl opcode. */
/*  354 */     OP_saddl2 = 354, /**< AArch64 saddl2 opcode. */
/*  355 */     OP_saddlp = 355, /**< AArch64 saddlp opcode. */
/*  356 */     OP_saddlv = 356, /**< AArch64 saddlv opcode. */
/*  357 */     OP_saddw = 357, /**< AArch64 saddw opcode. */
/*  358 */     OP_saddw2 = 358, /**< AArch64 saddw2 opcode. */
/*  359 */     OP_sbc = 359, /**< AArch64 sbc opcode. */
/*  360 */     OP_sbcs = 360, /**< AArch64 sbcs opcode. */
/*  361 */     OP_sbfm = 361, /**< AArch64 sbfm opcode. */
/*  362 */     OP_scvtf = 362, /**< AArch64 scvtf opcode. */
/*  363 */     OP_sdiv = 363, /**< AArch64 sdiv opcode. */
/*  364 */     OP_sdot = 364, /**< AArch64 sdot opcode. */
/*  365 */     OP_sev = 365, /**< AArch64 sev opcode. */
/*  366 */     OP_sevl = 366, /**< AArch64 sevl opcode. */
/*  367 */     OP_sha1c = 367, /**< AArch64 sha1c opcode. */
/*  368 */     OP_sha1h = 368, /**< AArch64 sha1h opcode. */
/*  369 */     OP_sha1m = 369, /**< AArch64 sha1m opcode. */
/*  370 */     OP_sha1p = 370, /**< AArch64 sha1p opcode. */
/*  371 */     OP_sha1su0 = 371, /**< AArch64 sha1su0 opcode. */
/*  372 */     OP_sha1su1 = 372, /**< AArch64 sha1su1 opcode. */
/*  373 */     OP_sha256h = 373, /**< AArch64 sha256h opcode. */
/*  374 */     OP_sha256h2 = 374, /**< AArch64 sha256h2 opcode. */
/*  375 */     OP_sha256su0 = 375, /**< AArch64 sha256su0 opcode. */
/*  376 */     OP_sha256su1 = 376, /**< AArch64 sha256su1 opcode. */
/*  377 */     OP_shadd = 377, /**< AArch64 shadd opcode. */
/*  378 */     OP_shl = 378, /**< AArch64 shl opcode. */
/*  379 */     OP_shll = 379, /**< AArch64 shll opcode. */
/*  380 */     OP_shll2 = 380, /**< AArch64 shll2 opcode. */
/*  381 */     OP_shrn = 381, /**< AArch64 shrn opcode. */
/*  382 */     OP_shrn2 = 382, /**< AArch64 shrn2 opcode. */
/*  383 */     OP_shsub = 383, /**< AArch64 shsub opcode. */
/*  384 */     OP_sli = 384, /**< AArch64 sli opcode. */
/*  385 */     OP_smaddl = 385, /**< AArch64 smaddl opcode. */
/*  386 */     OP_smax = 386, /**< AArch64 smax opcode. */
/*  387 */     OP_smaxp = 387, /**< AArch64 smaxp opcode. */
/*  388 */     OP_smaxv = 388, /**< AArch64 smaxv opcode. */
/*  389 */     OP_smc = 389, /**< AArch64 smc opcode. */
/*  390 */     OP_smin = 390, /**< AArch64 smin opcode. */
/*  391 */     OP_sminp = 391, /**< AArch64 sminp opcode. */
/*  392 */     OP_sminv = 392, /**< AArch64 sminv opcode. */
/*  393 */     OP_smlal = 393, /**< AArch64 smlal opcode. */
/*  394 */     OP_smlal2 = 394, /**< AArch64 smlal2 opcode. */
/*  395 */     OP_smlsl = 395, /**< AArch64 smlsl opcode. */
/*  396 */     OP_smlsl2 = 396, /**< AArch64 smlsl2 opcode. */
/*  397 */     OP_smov = 397, /**< AArch64 smov opcode. */
/*  398 */     OP_smsubl = 398, /**< AArch64 smsubl opcode. */
/*  399 */     OP_smulh = 399, /**< AArch64 smulh opcode. */
/*  400 */     OP_smull = 400, /**< AArch64 smull opcode. */
/*  401 */     OP_smull2 = 401, /**< AArch64 smull2 opcode. */
/*  402 */     OP_sqabs = 402, /**< AArch64 sqabs opcode. */
/*  403 */     OP_sqadd = 403, /**< AArch64 sqadd opcode. */
/*  404 */     OP_sqdmlal = 404, /**< AArch64 sqdmlal opcode. */
/*  405 */     OP_sqdmlal2 = 405, /**< AArch64 sqdmlal2 opcode. */
/*  406 */     OP_sqdmlsl = 406, /**< AArch64 sqdmlsl opcode. */
/*  407 */     OP_sqdmlsl2 = 407, /**< AArch64 sqdmlsl2 opcode. */
/*  408 */     OP_sqdmulh = 408, /**< AArch64 sqdmulh opcode. */
/*  409 */     OP_sqdmull = 409, /**< AArch64 sqdmull opcode. */
/*  410 */     OP_sqdmull2 = 410, /**< AArch64 sqdmull2 opcode. */
/*  411 */     OP_sqneg = 411, /**< AArch64 sqneg opcode. */
/*  412 */     OP_sqrdmlah = 412, /**< AArch64 sqrdmlah opcode. */
/*  413 */     OP_sqrdmulh = 413, /**< AArch64 sqrdmulh opcode. */
/*  414 */     OP_sqrshl = 414, /**< AArch64 sqrshl opcode. */
/*  415 */     OP_sqrshrn = 415, /**< AArch64 sqrshrn opcode. */
/*  416 */     OP_sqrshrn2 = 416, /**< AArch64 sqrshrn2 opcode. */
/*  417 */     OP_sqrshrun = 417, /**< AArch64 sqrshrun opcode. */
/*  418 */     OP_sqrshrun2 = 418, /**< AArch64 sqrshrun2 opcode. */
/*  419 */     OP_sqshl = 419, /**< AArch64 sqshl opcode. */
/*  420 */     OP_sqshlu = 420, /**< AArch64 sqshlu opcode. */
/*  421 */     OP_sqshrn = 421, /**< AArch64 sqshrn opcode. */
/*  422 */     OP_sqshrn2 = 422, /**< AArch64 sqshrn2 opcode. */
/*  423 */     OP_sqshrun = 423, /**< AArch64 sqshrun opcode. */
/*  424 */     OP_sqshrun2 = 424, /**< AArch64 sqshrun2 opcode. */
/*  425 */     OP_sqsub = 425, /**< AArch64 sqsub opcode. */
/*  426 */     OP_sqxtn = 426, /**< AArch64 sqxtn opcode. */
/*  427 */     OP_sqxtn2 = 427, /**< AArch64 sqxtn2 opcode. */
/*  428 */     OP_sqxtun = 428, /**< AArch64 sqxtun opcode. */
/*  429 */     OP_sqxtun2 = 429, /**< AArch64 sqxtun2 opcode. */
/*  430 */     OP_srhadd = 430, /**< AArch64 srhadd opcode. */
/*  431 */     OP_sri = 431, /**< AArch64 sri opcode. */
/*  432 */     OP_srshl = 432, /**< AArch64 srshl opcode. */
/*  433 */     OP_srshr = 433, /**< AArch64 srshr opcode. */
/*  434 */     OP_srsra = 434, /**< AArch64 srsra opcode. */
/*  435 */     OP_sshl = 435, /**< AArch64 sshl opcode. */
/*  436 */     OP_sshll = 436, /**< AArch64 sshll opcode. */
/*  437 */     OP_sshll2 = 437, /**< AArch64 sshll2 opcode. */
/*  438 */     OP_sshr = 438, /**< AArch64 sshr opcode. */
/*  439 */     OP_ssra = 439, /**< AArch64 ssra opcode. */
/*  440 */     OP_ssubl = 440, /**< AArch64 ssubl opcode. */
/*  441 */     OP_ssubl2 = 441, /**< AArch64 ssubl2 opcode. */
/*  442 */     OP_ssubw = 442, /**< AArch64 ssubw opcode. */
/*  443 */     OP_ssubw2 = 443, /**< AArch64 ssubw2 opcode. */
/*  444 */     OP_st1 = 444, /**< AArch64 st1 opcode. */
/*  445 */     OP_st2 = 445, /**< AArch64 st2 opcode. */
/*  446 */     OP_st3 = 446, /**< AArch64 st3 opcode. */
/*  447 */     OP_st4 = 447, /**< AArch64 st4 opcode. */
/*  448 */     OP_stlr = 448, /**< AArch64 stlr opcode. */
/*  449 */     OP_stlrb = 449, /**< AArch64 stlrb opcode. */
/*  450 */     OP_stlrh = 450, /**< AArch64 stlrh opcode. */
/*  451 */     OP_stlxp = 451, /**< AArch64 stlxp opcode. */
/*  452 */     OP_stlxr = 452, /**< AArch64 stlxr opcode. */
/*  453 */     OP_stlxrb = 453, /**< AArch64 stlxrb opcode. */
/*  454 */     OP_stlxrh = 454, /**< AArch64 stlxrh opcode. */
/*  455 */     OP_stnp = 455, /**< AArch64 stnp opcode. */
/*  456 */     OP_stp = 456, /**< AArch64 stp opcode. */
/*  457 */     OP_str = 457, /**< AArch64 str opcode. */
/*  458 */     OP_strb = 458, /**< AArch64 strb opcode. */
/*  459 */     OP_strh = 459, /**< AArch64 strh opcode. */
/*  460 */     OP_sttr = 460, /**< AArch64 sttr opcode. */
/*  461 */     OP_sttrb = 461, /**< AArch64 sttrb opcode. */
/*  462 */     OP_sttrh = 462, /**< AArch64 sttrh opcode. */
/*  463 */     OP_stur = 463, /**< AArch64 stur opcode. */
/*  464 */     OP_sturb = 464, /**< AArch64 sturb opcode. */
/*  465 */     OP_sturh = 465, /**< AArch64 sturh opcode. */
/*  466 */     OP_stxp = 466, /**< AArch64 stxp opcode. */
/*  467 */     OP_stxr = 467, /**< AArch64 stxr opcode. */
/*  468 */     OP_stxrb = 468, /**< AArch64 stxrb opcode. */
/*  469 */     OP_stxrh = 469, /**< AArch64 stxrh opcode. */
/*  470 */     OP_sub = 470, /**< AArch64 sub opcode. */
/*  471 */     OP_subhn = 471, /**< AArch64 subhn opcode. */
/*  472 */     OP_subhn2 = 472, /**< AArch64 subhn2 opcode. */
/*  473 */     OP_subs = 473, /**< AArch64 subs opcode. */
/*  474 */     OP_suqadd = 474, /**< AArch64 suqadd opcode. */
/*  475 */     OP_svc = 475, /**< AArch64 svc opcode. */
/*  476 */     OP_swp = 476, /**< AArch64 swp opcode. */
/*  477 */     OP_swpa = 477, /**< AArch64 swpa opcode. */
/*  478 */     OP_swpab = 478, /**< AArch64 swpab opcode. */
/*  479 */     OP_swpah = 479, /**< AArch64 swpah opcode. */
/*  480 */     OP_swpal = 480, /**< AArch64 swpal opcode. */
/*  481 */     OP_swpalb = 481, /**< AArch64 swpalb opcode. */
/*  482 */     OP_swpalh = 482, /**< AArch64 swpalh opcode. */
/*  483 */     OP_swpb = 483, /**< AArch64 swpb opcode. */
/*  484 */     OP_swph = 484, /**< AArch64 swph opcode. */
/*  485 */     OP_swpl = 485, /**< AArch64 swpl opcode. */
/*  486 */     OP_swplb = 486, /**< AArch64 swplb opcode. */
/*  487 */     OP_swplh = 487, /**< AArch64 swplh opcode. */
/*  488 */     OP_sys = 488, /**< AArch64 sys opcode. */
/*  489 */     OP_sysl = 489, /**< AArch64 sysl opcode. */
/*  490 */     OP_tbl = 490, /**< AArch64 tbl opcode. */
/*  491 */     OP_tbnz = 491, /**< AArch64 tbnz opcode. */
/*  492 */     OP_tbx = 492, /**< AArch64 tbx opcode. */
/*  493 */     OP_tbz = 493, /**< AArch64 tbz opcode. */
/*  494 */     OP_trn1 = 494, /**< AArch64 trn1 opcode. */
/*  495 */     OP_trn2 = 495, /**< AArch64 trn2 opcode. */
/*  496 */     OP_uaba = 496, /**< AArch64 uaba opcode. */
/*  497 */     OP_uabal = 497, /**< AArch64 uabal opcode. */
/*  498 */     OP_uabal2 = 498, /**< AArch64 uabal2 opcode. */
/*  499 */     OP_uabd = 499, /**< AArch64 uabd opcode. */
/*  500 */     OP_uabdl = 500, /**< AArch64 uabdl opcode. */
/*  501 */     OP_uabdl2 = 501, /**< AArch64 uabdl2 opcode. */
/*  502 */     OP_uadalp = 502, /**< AArch64 uadalp opcode. */
/*  503 */     OP_uaddl = 503, /**< AArch64 uaddl opcode. */
/*  504 */     OP_uaddl2 = 504, /**< AArch64 uaddl2 opcode. */
/*  505 */     OP_uaddlp = 505, /**< AArch64 uaddlp opcode. */
/*  506 */     OP_uaddlv = 506, /**< AArch64 uaddlv opcode. */
/*  507 */     OP_uaddw = 507, /**< AArch64 uaddw opcode. */
/*  508 */     OP_uaddw2 = 508, /**< AArch64 uaddw2 opcode. */
/*  509 */     OP_ubfm = 509, /**< AArch64 ubfm opcode. */
/*  510 */     OP_ucvtf = 510, /**< AArch64 ucvtf opcode. */
/*  511 */     OP_udiv = 511, /**< AArch64 udiv opcode. */
/*  512 */     OP_udot = 512, /**< AArch64 udot opcode. */
/*  513 */     OP_uhadd = 513, /**< AArch64 uhadd opcode. */
/*  514 */     OP_uhsub = 514, /**< AArch64 uhsub opcode. */
/*  515 */     OP_umaddl = 515, /**< AArch64 umaddl opcode. */
/*  516 */     OP_umax = 516, /**< AArch64 umax opcode. */
/*  517 */     OP_umaxp = 517, /**< AArch64 umaxp opcode. */
/*  518 */     OP_umaxv = 518, /**< AArch64 umaxv opcode. */
/*  519 */     OP_umin = 519, /**< AArch64 umin opcode. */
/*  520 */     OP_uminp = 520, /**< AArch64 uminp opcode. */
/*  521 */     OP_uminv = 521, /**< AArch64 uminv opcode. */
/*  522 */     OP_umlal = 522, /**< AArch64 umlal opcode. */
/*  523 */     OP_umlal2 = 523, /**< AArch64 umlal2 opcode. */
/*  524 */     OP_umlsl = 524, /**< AArch64 umlsl opcode. */
/*  525 */     OP_umlsl2 = 525, /**< AArch64 umlsl2 opcode. */
/*  526 */     OP_umov = 526, /**< AArch64 umov opcode. */
/*  527 */     OP_umsubl = 527, /**< AArch64 umsubl opcode. */
/*  528 */     OP_umulh = 528, /**< AArch64 umulh opcode. */
/*  529 */     OP_umull = 529, /**< AArch64 umull opcode. */
/*  530 */     OP_umull2 = 530, /**< AArch64 umull2 opcode. */
/*  531 */     OP_uqadd = 531, /**< AArch64 uqadd opcode. */
/*  532 */     OP_uqrshl = 532, /**< AArch64 uqrshl opcode. */
/*  533 */     OP_uqrshrn = 533, /**< AArch64 uqrshrn opcode. */
/*  534 */     OP_uqrshrn2 = 534, /**< AArch64 uqrshrn2 opcode. */
/*  535 */     OP_uqshl = 535, /**< AArch64 uqshl opcode. */
/*  536 */     OP_uqshrn = 536, /**< AArch64 uqshrn opcode. */
/*  537 */     OP_uqshrn2 = 537, /**< AArch64 uqshrn2 opcode. */
/*  538 */     OP_uqsub = 538, /**< AArch64 uqsub opcode. */
/*  539 */     OP_uqxtn = 539, /**< AArch64 uqxtn opcode. */
/*  540 */     OP_uqxtn2 = 540, /**< AArch64 uqxtn2 opcode. */
/*  541 */     OP_urecpe = 541, /**< AArch64 urecpe opcode. */
/*  542 */     OP_urhadd = 542, /**< AArch64 urhadd opcode. */
/*  543 */     OP_urshl = 543, /**< AArch64 urshl opcode. */
/*  544 */     OP_urshr = 544, /**< AArch64 urshr opcode. */
/*  545 */     OP_ursqrte = 545, /**< AArch64 ursqrte opcode. */
/*  546 */     OP_ursra = 546, /**< AArch64 ursra opcode. */
/*  547 */     OP_ushl = 547, /**< AArch64 ushl opcode. */
/*  548 */     OP_ushll = 548, /**< AArch64 ushll opcode. */
/*  549 */     OP_ushll2 = 549, /**< AArch64 ushll2 opcode. */
/*  550 */     OP_ushr = 550, /**< AArch64 ushr opcode. */
/*  551 */     OP_usqadd = 551, /**< AArch64 usqadd opcode. */
/*  552 */     OP_usra = 552, /**< AArch64 usra opcode. */
/*  553 */     OP_usubl = 553, /**< AArch64 usubl opcode. */
/*  554 */     OP_usubl2 = 554, /**< AArch64 usubl2 opcode. */
/*  555 */     OP_usubw = 555, /**< AArch64 usubw opcode. */
/*  556 */     OP_usubw2 = 556, /**< AArch64 usubw2 opcode. */
/*  557 */     OP_uzp1 = 557, /**< AArch64 uzp1 opcode. */
/*  558 */     OP_uzp2 = 558, /**< AArch64 uzp2 opcode. */
/*  559 */     OP_wfe = 559, /**< AArch64 wfe opcode. */
/*  560 */     OP_wfi = 560, /**< AArch64 wfi opcode. */
/*  561 */     OP_xpaclri = 561, /**< AArch64 xpaclri opcode. */
/*  562 */     OP_xtn = 562, /**< AArch64 xtn opcode. */
/*  563 */     OP_xtn2 = 563, /**< AArch64 xtn2 opcode. */
/*  564 */     OP_yield = 564, /**< AArch64 yield opcode. */
/*  565 */     OP_zip1 = 565, /**< AArch64 zip1 opcode. */
/*  566 */     OP_zip2 = 566, /**< AArch64 zip2 opcode. */
/*  567 */     OP_udf = 567, /**< AArch64 udf opcode. */
/*  568 */     OP_dc_zva = 568, /**< AArch64 dc_zva opcode. */
/*  569 */     OP_dc_cisw = 569, /**< AArch64 dc_cisw opcode. */
/*  570 */     OP_dc_civac = 570, /**< AArch64 dc_civac opcode. */
/*  571 */     OP_dc_csw = 571, /**< AArch64 dc_csw opcode. */
/*  572 */     OP_dc_cvac = 572, /**< AArch64 dc_cvac opcode. */
/*  573 */     OP_dc_cvau = 573, /**< AArch64 dc_cvau opcode. */
/*  574 */     OP_dc_isw = 574, /**< AArch64 dc_isw opcode. */
/*  575 */     OP_dc_ivac = 575, /**< AArch64 dc_ivac opcode. */
/*  576 */     OP_ic_iallu = 576, /**< AArch64 ic_iallu opcode. */
/*  577 */     OP_ic_ialluis = 577, /**< AArch64 ic_ialluis opcode. */
/*  578 */     OP_ic_ivau = 578, /**< AArch64 ic_ivau opcode. */


    OP_AFTER_LAST,
    OP_FIRST = OP_LABEL + 1,      /**< First real opcode. */
    OP_LAST  = OP_AFTER_LAST - 1, /**< Last real opcode. */
};

/* alternative names */
#define OP_load      OP_ldr    /**< Platform-independent opcode name for load. */
#define OP_store     OP_str    /**< Platform-independent opcode name for store. */

/* alternative names */
#define OP_ldmia OP_ldm         /**< Alternative opcode name for ldm. */
#define OP_rfeia OP_rfe         /**< Alternative opcode name for rfe. */
#define OP_srsia OP_srs         /**< Alternative opcode name for srs. */
#define OP_stmia OP_stm         /**< Alternative opcode name for stm. */
#define OP_vldmia OP_vldm       /**< Alternative opcode name for vldm. */
#define OP_vstmia OP_vstm       /**< Alternative opcode name for vstm. */
#define OP_fldmx OP_vldm        /**< Alternative opcode name for vldm. */
#define OP_fstmx OP_vstm        /**< Alternative opcode name for vstm. */
#define OP_pop OP_ldr           /**< Alternative opcode name for ldr. */
#define OP_push OP_stmdb        /**< Alternative opcode name for stmdb. */
#define OP_vpop OP_vldmia       /**< Alternative opcode name for vldmia. */
#define OP_vpush OP_vstmdb      /**< Alternative opcode name for vstmdb. */
#define OP_cpy OP_mov           /**< Alternative opcode name for reg-reg mov. */
#define OP_jmp OP_b             /**< Platform-independent opcode name for jump. */
#define OP_jmp_short OP_b_short /**< Platform-independent opcode name for short jump. */
#define OP_load OP_ldr          /**< Platform-independent opcode name for load. */
#define OP_store OP_str         /**< Platform-independent opcode name for store. */

struct _opnd_t;
typedef struct _opnd_t opnd_t;
struct _instr_t;
typedef struct _instr_t instr_t;

typedef struct _dr_instr_label_data_t {
    ptr_uint_t data[4]; /**< Generic fields for storing user-controlled data */
} dr_instr_label_data_t;

typedef void (*instr_label_callback_t)(instr_t *instr);

/**
 * opnd_t type exposed for optional "fast IR" access.  Note that DynamoRIO
 * reserves the right to change this structure across releases and does
 * not guarantee binary or source compatibility when this structure's fields
 * are directly accessed.  If the OPND_ macros are used, DynamoRIO does
 * guarantee source compatibility, but not binary compatibility.  If binary
 * compatibility is desired, do not use the fast IR feature.
 */
struct _opnd_t {
    byte kind;
    /* Size field: used for immed_ints and addresses and registers,
     * but for registers, if 0, the full size of the register is assumed.
     * It holds a OPSZ_ field from decode.h.
     * We need it so we can pick the proper instruction form for
     * encoding -- an alternative would be to split all the opcodes
     * up into different data size versions.
     */
    opnd_size_t size;
    /* To avoid increasing our union beyond 64 bits, we store additional data
     * needed for x64 operand types here in the alignment padding.
     */
    union {
        ushort far_pc_seg_selector; /* FAR_PC_kind and FAR_INSTR_kind */
        /* We could fit segment in value.base_disp but more consistent here */
        reg_id_t segment : REG_SPECIFIER_BITS; /* BASE_DISP_kind, REL_ADDR_kind,
                                                * and ABS_ADDR_kind, on x86 */
        ushort disp;                           /* MEM_INSTR_kind */
        ushort shift;                          /* INSTR_kind */
        /* We have to use a shorter type, not the enum type, to get cl to not align */
        /* Used for ARM: REG_kind, BASE_DISP_kind, and IMMED_INTEGER_kind */
        ushort /*dr_opnd_flags_t*/ flags;
    } aux;
    union {
        /* all are 64 bits or less */
        /* NULL_kind has no value */
        ptr_int_t immed_int; /* IMMED_INTEGER_kind */
        struct {
            int low;  /* IMMED_INTEGER_kind with DR_OPND_MULTI_PART */
            int high; /* IMMED_INTEGER_kind with DR_OPND_MULTI_PART */
        } immed_int_multi_part;
        float immed_float; /* IMMED_FLOAT_kind */
#    ifndef WINDOWS
        /* XXX i#4488: x87 floating point immediates should be double precision.
         * Currently not included for Windows because sizeof(opnd_t) does not
         * equal EXPECTED_SIZEOF_OPND, triggering the ASSERT in d_r_arch_init().
         */
        /* For 32-bit ARM we keep alignment at 4 to avoid changing the opnd_t shape.
         * Marking this field as packed seems to do it and avoids other changes
         * that might occur if packing the whole struct.
         * XXX i#4488: Do any double-loading instructions require 8-byte alignment?
         * Perhaps we should just break compatibility and align this to 8 for
         * x86 and ARM 32-bit.
         */
        double immed_double IF_ARM(__attribute__((__packed__))); /* IMMED_DOUBLE_kind */
#    endif
        /* PR 225937: today we provide no way of specifying a 16-bit immediate
         * (encoded as a data16 prefix, which also implies a 16-bit EIP,
         * making it only useful for far pcs)
         */
        app_pc pc; /* PC_kind and FAR_PC_kind */
        /* For FAR_PC_kind and FAR_INSTR_kind, we use pc/instr, and keep the
         * segment selector (which is NOT a DR_SEG_ constant) in far_pc_seg_selector
         * above, to save space.
         */
        instr_t *instr; /* INSTR_kind, FAR_INSTR_kind, and MEM_INSTR_kind */
        reg_id_t reg;   /* REG_kind */
        struct {
            /* For ARM, either disp==0 or index_reg==DR_REG_NULL: can't have both */
            int disp;
            reg_id_t base_reg : REG_SPECIFIER_BITS;
            reg_id_t index_reg : REG_SPECIFIER_BITS;
            /* to get cl to not align to 4 bytes we can't use uint here
             * when we have reg_id_t elsewhere: it won't combine them
             * (gcc will). alternative is all uint and no reg_id_t.
             * We also have to use byte and not dr_shift_type_t
             * to get cl to not align.
             */
#    if defined(AARCH64)
            /* This is only used to distinguish pre-index from post-index when the
             * offset is zero, for example: ldr w1,[x2,#0]! from ldr w1,[x0],#0.
             */
            byte /*bool*/ pre_index : 1;
            /* Access this using opnd_get_index_extend and opnd_set_index_extend. */
            byte /*dr_extend_type_t*/ extend_type : 3;
            /* Shift register offset left by amount implied by size of memory operand: */
            byte /*bool*/ scaled : 1;
#    elif defined(ARM)
            byte /*dr_shift_type_t*/ shift_type : 3;
            byte shift_amount_minus_1 : 5; /* 1..31 so we store (val - 1) */
#    elif defined(X86)
            byte scale : SCALE_SPECIFIER_BITS;
            byte /*bool*/ encode_zero_disp : 1;
            byte /*bool*/ force_full_disp : 1;  /* don't use 8-bit even w/ 8-bit value */
            byte /*bool*/ disp_short_addr : 1;  /* 16-bit (32 in x64) addr (disp-only) */
            byte /*bool*/ index_reg_is_zmm : 1; /* Indicates that the index_reg of the
                                                 * VSIB address is of length ZMM. This
                                                 * flag is not exposed and serves as an
                                                 * internal AVX-512 extension of
                                                 * index_reg, leaving index_reg binary
                                                 * compatible at 8 bits.
                                                 */
#    endif
        } base_disp; /* BASE_DISP_kind */
        void *addr;  /* REL_ADDR_kind and ABS_ADDR_kind */
    } value;
};

typedef struct _decode_info_t {
    bool check_reachable; //This is used by the encoder, not the decoder.
} decode_info_t;
/*
typedef struct decode_info_t
{
    int index;
    bool innovative;
} decode_info_t;
*/

/**
 * instr_t type exposed for optional "fast IR" access.  Note that DynamoRIO
 * reserves the right to change this structure across releases and does
 * not guarantee binary or source compatibility when this structure's fields
 * are directly accessed.  If the instr_ accessor routines are used, DynamoRIO does
 * guarantee source compatibility, but not binary compatibility.  If binary
 * compatibility is desired, do not use the fast IR feature.
 */
struct _instr_t {
    /* flags contains the constants defined above */
    uint flags;

    /* hints for encoding this instr in a specific way, holds dr_encoding_hint_type_t */
    uint encoding_hints;

    /* Raw bits of length length are pointed to by the bytes field.
     * label_cb stores a callback function pointer used by label instructions
     * and called when the label is freed.
     */
    uint length;
    union {
        byte *bytes;
        instr_label_callback_t label_cb;
    };

    /* translation target for this instr */
    app_pc translation;

    uint opcode;

    /* we dynamically allocate dst and src arrays b/c x86 instrs can have
     * up to 8 of each of them, but most have <=2 dsts and <=3 srcs, and we
     * use this struct for un-decoded instrs too
     */
    byte num_dsts;
    byte num_srcs;

    union {
        struct {
            /* for efficiency everyone has a 1st src opnd, since we often just
             * decode jumps, which all have a single source (==target)
             * yes this is an extra 10 bytes, but the whole struct is still < 64 bytes!
             */
            opnd_t src0;
            opnd_t *srcs; /* this array has 2nd src and beyond */
            opnd_t *dsts;
        };
        dr_instr_label_data_t label_data;
    };

    uint prefixes; /* data size, addr size, or lock prefix info */
    uint eflags;   /* contains EFLAGS_ bits, but amount of info varies
                    * depending on how instr was decoded/built */

    /* this field is for the use of passes as an annotation.
     * it is also used to hold the offset of an instruction when encoding
     * pc-relative instructions. A small range of values is reserved for internal use
     * by DR and cannot be used by clients; see DR_NOTE_FIRST_RESERVED in globals.h.
     */
    void *note;

    /* fields for building instructions into instruction lists */
    instr_t *prev;
    instr_t *next;

};     /* instr_t */

enum {
#ifdef X86
    MAX_INSTR_LENGTH = 17,
    MAX_SRC_OPNDS = 8, /* pusha */
    MAX_DST_OPNDS = 8, /* popa */
#elif defined(AARCH64)
    /* The maximum instruction length is 64 to allow for an OP_ldstex containing
     * up to 16 real instructions. The longest such block seen so far in real
     * code had 7 instructions so this is likely to be enough. With the current
     * implementation, a larger value would significantly slow down the search
     * for such blocks in the decoder: see decode_ldstex().
     */
    MAX_INSTR_LENGTH = 64,
    MAX_SRC_OPNDS = 8,
    MAX_DST_OPNDS = 8,
#elif defined(ARM)
    MAX_INSTR_LENGTH = 4,
    /* With register lists we can see quite long operand lists. */
    MAX_SRC_OPNDS = 33, /* vstm s0-s31 */
    MAX_DST_OPNDS = MAX_SRC_OPNDS,
#endif
};


typedef struct instr_noalloc_t {
    instr_t instr; /**< The base instruction, valid for passing to API functions. */
    opnd_t srcs[MAX_SRC_OPNDS - 1];    /**< Built-in storage for source operands. */
    opnd_t dsts[MAX_DST_OPNDS];        /**< Built-in storage for destination operands. */
    byte encode_buf[MAX_INSTR_LENGTH]; /**< Encoding space for instr_length(), etc. */
} instr_noalloc_t;

#define ALIGNED(x, alignment) ((((ptr_uint_t)x) & ((alignment)-1)) == 0)
#define ALIGN_BACKWARD(x, alignment) (((ptr_uint_t)x) & (~((alignment)-1)))
#define ALIGN_FORWARD(x, alignment) \
    ((((ptr_uint_t)x) + ((alignment)-1)) & (~((alignment)-1)))


/**
 * Used in an additional immediate source operand to a vector operation, denotes
 * 8 bit vector element width. See \ref sec_IR_AArch64.
 */
#define VECTOR_ELEM_WIDTH_BYTE 0

/**
 * Used in an additional immediate source operand to a vector operation, denotes
 * 16 bit vector element width. See \ref sec_IR_AArch64.
 */
#define VECTOR_ELEM_WIDTH_HALF 1

/**
 * Used in an additional immediate source operand to a vector operation, denotes
 * 32 bit vector element width. See \ref sec_IR_AArch64.
 */
#define VECTOR_ELEM_WIDTH_SINGLE 2

/**
 * Used in an additional immediate source operand to a vector operation, denotes
 * 64 bit vector element width. See \ref sec_IR_AArch64.
 */
#define VECTOR_ELEM_WIDTH_DOUBLE 3

/**
 * Used in an additional immediate source operand to a vector operation, denotes
 * full size 128 bit vector width. See \ref sec_IR_AArch64.
 */
#define VECTOR_ELEM_WIDTH_QUAD 4

#ifdef assert
#    undef assert
#endif
/* avoid mistake of lower-case assert */
#define assert assert_no_good_use_ASSERT_instead

/* we provide a less-than-default level of checking */
#define CHKLVL_ASSERTS 1
#define CHKLVL_DEFAULT 2
#if defined(DEBUG) && !defined(NOT_DYNAMORIO_CORE_PROPER) && \
    !defined(NOT_DYNAMORIO_CORE) && !defined(STANDALONE_DECODER)
/* we can't use DYNAMO_OPTION() b/c that has an assert inside it */
#    define DEBUG_CHECKS(level) (dynamo_options.checklevel >= (level))
#else
#    define DEBUG_CHECKS(level) true
#endif

#if defined(DEBUG) && !defined(STANDALONE_DECODER)
#    ifdef INTERNAL
/* cast to void to avoid gcc warning "statement with no effect" when used as
 * a statement and x is a compile-time false
 * FIXME: cl debug build allocates a local for each ?:, so if this gets
 * unrolled in some optionsx or other expansion we could have stack problems!
 */
#        define ASSERT(x)                                                 \
            ((void)((DEBUG_CHECKS(CHKLVL_ASSERTS) && !(x))                \
                        ? (d_r_internal_error(__FILE__, __LINE__, #x), 0) \
                        : 0))
/* make type void to avoid gcc 4.1 warnings about "value computed is not used"
 * (case 7851).  can also use statement expr ({;}) but only w/ gcc, not w/ cl.
 */
#        define ASSERT_MESSAGE(level, msg, x)                                     \
            ((DEBUG_CHECKS(level) && !(x))                                        \
                 ? (d_r_internal_error(msg " @" __FILE__, __LINE__, #x), (void)0) \
                 : (void)0)
#        define REPORT_CURIOSITY(x)                                                   \
            do {                                                                      \
                if (!ignore_assert(__FILE__ ":" STRINGIFY(__LINE__),                  \
                                   "curiosity : " #x)) {                              \
                    report_dynamorio_problem(NULL, DUMPCORE_CURIOSITY, NULL, NULL,    \
                                             "CURIOSITY : %s in file %s line %d", #x, \
                                             __FILE__, __LINE__);                     \
                }                                                                     \
            } while (0)
#        define ASSERT_CURIOSITY(x)                         \
            do {                                            \
                if (DEBUG_CHECKS(CHKLVL_ASSERTS) && !(x)) { \
                    REPORT_CURIOSITY(x);                    \
                }                                           \
            } while (0)
#        define ASSERT_CURIOSITY_ONCE(x)                    \
            do {                                            \
                if (DEBUG_CHECKS(CHKLVL_ASSERTS) && !(x)) { \
                    DO_ONCE({ REPORT_CURIOSITY(x); });      \
                }                                           \
            } while (0)
#    else
/* cast to void to avoid gcc warning "statement with no effect" */
#        define ASSERT(x)                                                 \
            ((void)((DEBUG_CHECKS(CHKLVL_ASSERTS) && !(x))                \
                        ? (d_r_internal_error(__FILE__, __LINE__, ""), 0) \
                        : 0))
#        define ASSERT_MESSAGE(level, msg, x)                             \
            ((void)((DEBUG_CHECKS(level) && !(x))                         \
                        ? (d_r_internal_error(__FILE__, __LINE__, ""), 0) \
                        : 0))
#        define ASSERT_CURIOSITY(x) ((void)0)
#        define ASSERT_CURIOSITY_ONCE(x) ((void)0)
#    endif /* INTERNAL */
#    define ASSERT_NOT_TESTED() \
        SYSLOG_INTERNAL_WARNING_ONCE("Not tested @%s:%d", __FILE__, __LINE__)
#else
#    define ASSERT(x) ((void)0)
#    define ASSERT_MESSAGE(level, msg, x) ASSERT(x)
#    define ASSERT_NOT_TESTED() ASSERT(true)
#    define ASSERT_CURIOSITY(x) ASSERT(true)
#    define ASSERT_CURIOSITY_ONCE(x) ASSERT(true)
#endif /* DEBUG */

#define ASSERT_NOT_REACHED() ASSERT(false)
#define ASSERT_BUG_NUM(num, x) ASSERT_MESSAGE(CHKLVL_ASSERTS, "Bug #" #num, x)
#define ASSERT_NOT_IMPLEMENTED(x) ASSERT_MESSAGE(CHKLVL_ASSERTS, "Not implemented", x)

/* linkstub_t flags
 * WARNING: flags field is a ushort, so max flag is 0x8000!
 */
enum {
    /***************************************************/
    /* these first flags are also used on the instr_t data structure */

    /* Type of branch and thus which struct is used for this exit.
     * Due to a tight namespace (flags is a ushort field), we pack our
     * 3 types into these 2 bits, so the LINKSTUB_ macros are used to
     * distinguish, rather than raw bit tests:
     *
     *   name          LINK_DIRECT LINK_INDIRECT  struct
     *   ---------------  ---         ---         --------------------------
     *   (subset of fake)  0           0          linkstub_t
     *   normal direct     1           0          direct_linkstub_t
     *   normal indirect   0           1          indirect_linkstub_t
     *   cbr fallthrough   1           1          cbr_fallthrough_linkstub_t
     *
     * Note that we can have fake linkstubs that should be treated as
     * direct or indirect, so LINK_FAKE is a separate flag.
     */
    LINK_DIRECT = 0x0001,
    LINK_INDIRECT = 0x0002,
    /* more specifics on type of branch
     * must check LINK_DIRECT vs LINK_INDIRECT for JMP and CALL.
     * absence of all of these is relied on as an indicator of shared_syscall
     * in indirect_linkstub_target(), so we can't get rid of LINK_RETURN
     * and use absence of CALL & JMP to indicate it.
     */
    LINK_RETURN = 0x0004,
    /* JMP|CALL indicates JMP_PLT, so use EXIT_IS_{JMP,CALL} rather than these raw bits */
    LINK_CALL = 0x0008,
    LINK_JMP = 0x0010,

    /* Indicates a far cti which uses a separate ibl entry */
    LINK_FAR = 0x0020,

    /* This exit cti has a preceding NOP to avoid its immediate from crossing a cache
     * line.  This flag lets us identify NOPs which we added as opposed to the client.
     */
    LINK_PADDED = 0x0040,

#ifdef X64
    /* PR 257963: since we don't store targets of ind branches, we need a flag
     * so we know whether this is a trace cmp exit, which has its own ibl entry
     */
    LINK_TRACE_CMP = 0x0080,
#endif
    /* Flags that tell DR to take some action upon returning to d_r_dispatch.
     * This first one is multiplexed via dcontext->upcontext.upcontext.exit_reason.
     * All uses are assumed to be unlinkable.
     */
    LINK_SPECIAL_EXIT = 0x0100,
#ifdef WINDOWS
    LINK_CALLBACK_RETURN = 0x0200,
#else
    /* PR 286922: we support both OP_sys{call,enter}- and OP_int-based system calls */
    LINK_NI_SYSCALL_INT = 0x0200,
#endif
    /* indicates whether exit is before a non-ignorable syscall */
    LINK_NI_SYSCALL = 0x0400,
    LINK_FINAL_INSTR_SHARED_FLAG = LINK_NI_SYSCALL,
    /* end of instr_t-shared flags  */
    /***************************************************/

    LINK_FRAG_OFFS_AT_END = 0x0800,

    LINK_END_OF_LIST = 0x1000,

    LINK_FAKE = 0x2000,

    LINK_LINKED = 0x4000,

    LINK_SEPARATE_STUB = 0x8000,

    /* WARNING: flags field is a ushort, so max flag is 0x8000! */
};

#ifdef UNIX
#    define LINK_NI_SYSCALL_ALL (LINK_NI_SYSCALL | LINK_NI_SYSCALL_INT)
#else
#    define LINK_NI_SYSCALL_ALL LINK_NI_SYSCALL
#endif

/* flags */
enum {
    /* these first flags are shared with the LINK_ flags and are
     * used to pass on info to link stubs
     */
    /* used to determine type of indirect branch for exits */
    INSTR_DIRECT_EXIT = LINK_DIRECT,
    INSTR_INDIRECT_EXIT = LINK_INDIRECT,
    INSTR_RETURN_EXIT = LINK_RETURN,
    /* JMP|CALL marks an indirect jmp preceded by a call (== a PLT-style ind call)
     * so use EXIT_IS_{JMP,CALL} rather than these raw bits
     */
    INSTR_CALL_EXIT = LINK_CALL,
    INSTR_JMP_EXIT = LINK_JMP,
    INSTR_IND_JMP_PLT_EXIT = (INSTR_JMP_EXIT | INSTR_CALL_EXIT),
    INSTR_FAR_EXIT = LINK_FAR,
    INSTR_BRANCH_SPECIAL_EXIT = LINK_SPECIAL_EXIT,
    INSTR_BRANCH_PADDED = LINK_PADDED,
#ifdef X64
    /* PR 257963: since we don't store targets of ind branches, we need a flag
     * so we know whether this is a trace cmp exit, which has its own ibl entry
     */
    INSTR_TRACE_CMP_EXIT = LINK_TRACE_CMP,
#endif
#ifdef WINDOWS
    INSTR_CALLBACK_RETURN = LINK_CALLBACK_RETURN,
#else
    INSTR_NI_SYSCALL_INT = LINK_NI_SYSCALL_INT,
#endif
    INSTR_NI_SYSCALL = LINK_NI_SYSCALL,
    INSTR_NI_SYSCALL_ALL = LINK_NI_SYSCALL_ALL,
    /* meta-flag */
    EXIT_CTI_TYPES = (INSTR_DIRECT_EXIT | INSTR_INDIRECT_EXIT | INSTR_RETURN_EXIT |
                      INSTR_CALL_EXIT | INSTR_JMP_EXIT | INSTR_FAR_EXIT |
                      INSTR_BRANCH_SPECIAL_EXIT | INSTR_BRANCH_PADDED |
#ifdef X64
                      INSTR_TRACE_CMP_EXIT |
#endif
#ifdef WINDOWS
                      INSTR_CALLBACK_RETURN |
#else
                      INSTR_NI_SYSCALL_INT |
#endif
                      INSTR_NI_SYSCALL),

    /* instr_t-internal flags (not shared with LINK_) */
    INSTR_OPERANDS_VALID = 0x00010000,
    /* meta-flag */
    INSTR_FIRST_NON_LINK_SHARED_FLAG = INSTR_OPERANDS_VALID,
    INSTR_EFLAGS_VALID = 0x00020000,
    INSTR_EFLAGS_6_VALID = 0x00040000,
    INSTR_RAW_BITS_VALID = 0x00080000,
    INSTR_RAW_BITS_ALLOCATED = 0x00100000,
    /* This flag is in instr_api.h as it's needed for inlining support.
     *   INSTR_DO_NOT_MANGLE = 0x00200000,
     */
    /* This flag is set by instr_noalloc_init() and used to identify the
     * instr_noalloc_t "subclass" of instr_t.  It should not be otherwise used.
     */
    INSTR_IS_NOALLOC_STRUCT = 0x00400000,
    /* used to indicate that an indirect call can be treated as a direct call */
    INSTR_IND_CALL_DIRECT = 0x00800000,
#ifdef WINDOWS
    /* used to indicate that a syscall should be executed via shared syscall */
    INSTR_SHARED_SYSCALL = 0x01000000,
#else
    /* Indicates an instruction that's part of the rseq endpoint.  We use this in
     * instrlist_t.flags (sort of the same namespace: INSTR_OUR_MANGLING is used there,
     * but also EDI_VAL_*) and as a version of DR_NOTE_RSEQ that survives encoding
     * (seems like we could store notes for labels in another field so they do
     * in fact survive: a union with instr_t.translation?).
     */
    INSTR_RSEQ_ENDPOINT = 0x01000000,
#endif

    /* This enum is also used for INSTR_OUR_MANGLING_EPILOGUE. Its semantics are
     * orthogonal to this and must not overlap.
     */
    INSTR_CLOBBER_RETADDR = 0x02000000,

    /* Indicates that the instruction is part of an own mangling region's
     * epilogue (xref i#3307). Currently, instructions with the
     * INSTR_CLOBBER_RETADDR property are never in a mangling epilogue, which
     * is why we are reusing its enum value here.
     * */
    INSTR_OUR_MANGLING_EPILOGUE = 0x02000000,
    /* Signifies that this instruction may need to be hot patched and should
     * therefore not cross a cache line. It is not necessary to set this for
     * exit cti's or linkstubs since it is mainly intended for clients etc.
     * Handling of this flag is not yet implemented */
    INSTR_HOT_PATCHABLE = 0x04000000,
#ifdef DEBUG
    /* case 9151: only report invalid instrs for normal code decoding */
    INSTR_IGNORE_INVALID = 0x08000000,
#endif
    /* Currently used for frozen coarse fragments with final jmps and
     * jmps to ib stubs that are elided: we need the jmp instr there
     * to build the linkstub_t but we do not want to emit it. */
    INSTR_DO_NOT_EMIT = 0x10000000,
    /* PR 251479: re-relativization support: is instr->rip_rel_pos valid? */
    INSTR_RIP_REL_VALID = 0x20000000,
#ifdef X86
    /* PR 278329: each instr stores its own mode */
    INSTR_X86_MODE = 0x40000000,
#elif defined(ARM)
    /* We assume we don't need to distinguish A64 from A32 as you cannot swap
     * between them in user mode.  Thus we only need one flag.
     * XXX: we might want more power for drdecode, though the global isa_mode
     * should be sufficient there.
     */
    INSTR_THUMB_MODE = 0x40000000,
#endif
    /* PR 267260: distinguish our own mangling from client-added instrs */
    INSTR_OUR_MANGLING = 0x80000000,
};

/* moves the instruction from USE_ORIGINAL_BITS state to a
 * needs-full-encoding state
 */
void
instr_set_raw_bits_valid(instr_t *instr, bool valid)
{
    if (valid)
        instr->flags |= INSTR_RAW_BITS_VALID;
    else {
        instr->flags &= ~INSTR_RAW_BITS_VALID;
        /* DO NOT set bytes to NULL or length to 0, we still want to be
         * able to point at the original instruction for use in translating
         * addresses for exception/signal handlers
         * Also do not de-allocate allocated bits
         */
    }
}

byte *
instr_get_raw_bits(instr_t *instr)
{
    return instr->bytes;
}

DR_EXPORT
INSTR_INLINE
bool
instr_raw_bits_valid(instr_t *instr)
{
    return TEST(INSTR_RAW_BITS_VALID, instr->flags);
}

INSTR_INLINE
bool
instr_has_allocated_bits(instr_t *instr)
{
    return TEST(INSTR_RAW_BITS_ALLOCATED, instr->flags);
}

INSTR_INLINE
bool
instr_operands_valid(instr_t *instr)
{
    return TEST(INSTR_OPERANDS_VALID, instr->flags);
}

bool
instr_opcode_valid(instr_t *instr)
{
    return (instr->opcode != OP_INVALID && instr->opcode != OP_UNDECODED);
}

void
instr_set_our_mangling(instr_t *instr, bool ours)
{
    if (ours)
        instr->flags |= INSTR_OUR_MANGLING;
    else
        instr->flags &= ~INSTR_OUR_MANGLING;
}

static inline void
instr_being_modified(instr_t *instr, bool raw_bits_valid)
{
    if (!raw_bits_valid) {
        /* if we're modifying the instr, don't use original bits to encode! */
        instr_set_raw_bits_valid(instr, false);
    }
    /* PR 214962: if client changes our mangling, un-mark to avoid bad translation */
    instr_set_our_mangling(instr, false);
}

void
instr_set_opcode(instr_t *instr, int opcode)
{
    instr->opcode = opcode;
    /* if we're modifying opcode, don't use original bits to encode! */
    instr_being_modified(instr, false /*raw bits invalid*/);
    /* do not assume operands are valid, they are separate from opcode,
     * but if opcode is invalid operands shouldn't be valid
     */
    CLIENT_ASSERT((opcode != OP_INVALID && opcode != OP_UNDECODED) ||
                      !instr_operands_valid(instr),
                  "instr_set_opcode: operand-opcode validity mismatch");
}

void
instr_set_operands_valid(instr_t *instr, bool valid)
{
    if (valid)
        instr->flags |= INSTR_OPERANDS_VALID;
    else
        instr->flags &= ~INSTR_OPERANDS_VALID;
}

typedef enum {
    ACCT_FRAGMENT = 0,
    ACCT_COARSE_LINK,
    ACCT_FRAG_FUTURE,
    ACCT_FRAG_TABLE,
    ACCT_IBLTABLE,
    ACCT_TRACE,
    ACCT_FCACHE_EMPTY,
    ACCT_VMAREA_MULTI,
    ACCT_IR,
    ACCT_AFTER_CALL,
    ACCT_VMAREAS,
    ACCT_SYMBOLS,
#    ifdef SIDELINE
    ACCT_SIDELINE,
#    endif
    ACCT_THCOUNTER,
    ACCT_TOMBSTONE, /* N.B.: leaks in this category are not reported;
                     * not currently used */

    ACCT_HOT_PATCHING,
    ACCT_THREAD_MGT,
    ACCT_MEM_MGT,
    ACCT_STATS,
    ACCT_SPECIAL,
    ACCT_CLIENT,
    ACCT_LIBDUP, /* private copies of system libs => may leak */
    ACCT_CLEANCALL,
    /* NOTE: Also update the whichheap_name in heap.c when adding here */
    ACCT_OTHER,
    ACCT_LAST
} which_heap_t;

#    define HEAPACCT(x) , x
#    define IF_HEAPACCT_ELSE(x, y) x

void *
heap_alloc(size_t size HEAPACCT(which_heap_t which))
{
    return malloc(size);
}

void
heap_free(void *p, size_t size HEAPACCT(which_heap_t which))
{
    free(p);
}

void
heap_reachable_free(void *p,
                    size_t size HEAPACCT(which_heap_t which))
{
    free(p);
}

/* allocates storage for instr_num_srcs src operands and instr_num_dsts dst operands
 * assumes that instr is currently all zeroed out!
 */
void
instr_set_num_opnds(instr_t *instr, int instr_num_dsts,
                    int instr_num_srcs)
{
    if (instr_num_dsts > 0) {
        CLIENT_ASSERT(instr->num_dsts == 0 && instr->dsts == NULL,
                      "instr_set_num_opnds: dsts are already set");
        CLIENT_ASSERT_TRUNCATE(instr->num_dsts, byte, instr_num_dsts,
                               "instr_set_num_opnds: too many dsts");
        instr->num_dsts = (byte)instr_num_dsts;
        if (TEST(INSTR_IS_NOALLOC_STRUCT, instr->flags)) {
            instr_noalloc_t *noalloc = (instr_noalloc_t *)instr;
            noalloc->instr.dsts = noalloc->dsts;
        } else {
            instr->dsts = (opnd_t *)heap_alloc(instr_num_dsts * sizeof(opnd_t) HEAPACCT(ACCT_IR));
        }
    }
    if (instr_num_srcs > 0) {
        /* remember that src0 is static, rest are dynamic */
        if (instr_num_srcs > 1) {
            CLIENT_ASSERT(instr->num_srcs <= 1 && instr->srcs == NULL,
                          "instr_set_num_opnds: srcs are already set");
            if (TEST(INSTR_IS_NOALLOC_STRUCT, instr->flags)) {
                instr_noalloc_t *noalloc = (instr_noalloc_t *)instr;
                noalloc->instr.srcs = noalloc->srcs;
            } else {
                instr->srcs = (opnd_t *)heap_alloc((instr_num_srcs - 1) * sizeof(opnd_t) HEAPACCT(ACCT_IR));
            }
        }
        CLIENT_ASSERT_TRUNCATE(instr->num_srcs, byte, instr_num_srcs,
                               "instr_set_num_opnds: too many srcs");
        instr->num_srcs = (byte)instr_num_srcs;
    }
    instr_being_modified(instr, false /*raw bits invalid*/);
    /* assume all operands are valid */
    instr_set_operands_valid(instr, true);
}

/* sets the src opnd at position pos in instr */
void
instr_set_src(instr_t *instr, uint pos, opnd_t opnd)
{
    CLIENT_ASSERT(pos >= 0 && pos < instr->num_srcs, "instr_set_src: ordinal invalid");
    /* remember that src0 is static, rest are dynamic */
    if (pos == 0)
        instr->src0 = opnd;
    else
        instr->srcs[pos - 1] = opnd;
    /* if we're modifying operands, don't use original bits to encode! */
    instr_being_modified(instr, false /*raw bits invalid*/);
    /* assume all operands are valid */
    instr_set_operands_valid(instr, true);
}

/* sets the dst opnd at position pos in instr */
void
instr_set_dst(instr_t *instr, uint pos, opnd_t opnd)
{
    CLIENT_ASSERT(pos >= 0 && pos < instr->num_dsts, "instr_set_dst: ordinal invalid");
    instr->dsts[pos] = opnd;
    /* if we're modifying operands, don't use original bits to encode! */
    instr_being_modified(instr, false /*raw bits invalid*/);
    /* assume all operands are valid */
    instr_set_operands_valid(instr, true);
}

/** Specifies which processor mode to use when decoding or encoding. */
typedef enum _dr_isa_mode_t {
    DR_ISA_IA32,              /**< IA-32 (Intel/AMD 32-bit mode). */
    DR_ISA_X86 = DR_ISA_IA32, /**< Alias for DR_ISA_IA32. */
    DR_ISA_AMD64,             /**< AMD64 (Intel/AMD 64-bit mode). */
    DR_ISA_ARM_A32,           /**< ARM A32 (AArch32 ARM). */
    DR_ISA_ARM_THUMB,         /**< Thumb (ARM T32). */
    DR_ISA_ARM_A64,           /**< ARM A64 (AArch64). */
} dr_isa_mode_t;


/* static version for drdecodelib */
#define DEFAULT_ISA_MODE_STATIC                         \
    IF_X86_ELSE(IF_X64_ELSE(DR_ISA_AMD64, DR_ISA_IA32), \
                IF_X64_ELSE(DR_ISA_ARM_A64, DR_ISA_ARM_THUMB))

/* Use this one in DR proper.
 * This one is now static as well after we removed the runtime option that
 * used to be here: but I'm leaving the split to make it easier to add
 * an option in the future.
 */
#define DEFAULT_ISA_MODE                                \
    IF_X86_ELSE(IF_X64_ELSE(DR_ISA_AMD64, DR_ISA_IA32), \
                IF_X64_ELSE(DR_ISA_ARM_A64, DR_ISA_ARM_THUMB))

static dr_isa_mode_t initexit_isa_mode = DEFAULT_ISA_MODE_STATIC;

bool
is_isa_mode_legal(dr_isa_mode_t mode)
{
    return (mode == DR_ISA_ARM_A64);
}

/* The decode and encode routines use a per-thread persistent flag that
 * indicates which processor mode to use.  This routine returns the value of
 * that flag.
 */
dr_isa_mode_t
dr_get_isa_mode()
{
    /* Support GLOBAL_DCONTEXT or NULL for standalone/static modes */
        return initexit_isa_mode;
}

/* The decode and encode routines use a per-thread persistent flag that
 * indicates which processor mode to use.  This routine sets that flag to the
 * indicated value and optionally returns the old value.  Be sure to restore the
 * old value prior to any further application execution to avoid problems in
 * mis-interpreting application code.
 */
bool
dr_set_isa_mode(dr_isa_mode_t new_mode, dr_isa_mode_t *old_mode_out OUT)
{
    dr_isa_mode_t old_mode;
    /* Support GLOBAL_DCONTEXT or NULL for standalone/static modes */
    old_mode = initexit_isa_mode;
    if (is_isa_mode_legal(new_mode))
        initexit_isa_mode = new_mode;
    if (old_mode_out != NULL)
        *old_mode_out = old_mode;
    return is_isa_mode_legal(new_mode);
}

bool
instr_set_isa_mode(instr_t *instr, dr_isa_mode_t mode)
{
    return (mode == DR_ISA_ARM_A64);
}

dr_isa_mode_t
instr_get_isa_mode(instr_t *instr)
{
    return DR_ISA_ARM_A64;
}

/* For use in CLIENT_ASSERT or elsewhere that exists even if
 * STANDALONE_DECODER is defined.
 */
#ifdef DEBUG
#    define DEBUG_EXT_DECLARE(declaration) declaration
#else
#    define DEBUG_EXT_DECLARE(declaration)
#endif

void
instr_free_raw_bits(instr_t *instr)
{
    if ((instr->flags & INSTR_RAW_BITS_ALLOCATED) == 0)
        return;
    if (!TEST(INSTR_IS_NOALLOC_STRUCT, instr->flags))
        heap_reachable_free(instr->bytes, instr->length HEAPACCT(ACCT_IR));
    instr->bytes = NULL;
    instr->flags &= ~INSTR_RAW_BITS_VALID;
    instr->flags &= ~INSTR_RAW_BITS_ALLOCATED;
}

bool
instr_is_label(instr_t *instr);

INSTR_INLINE
bool
instr_is_app(instr_t *instr);

instr_label_callback_t
instr_get_label_callback(instr_t *instr)
{
    CLIENT_ASSERT(instr_is_label(instr),
                  "only label instructions have a callback function");
    CLIENT_ASSERT(!TEST(INSTR_RAW_BITS_ALLOCATED, instr->flags),
                  "instruction's raw bits occupying label callback memory");
    return instr->label_cb;
}

/* Frees all dynamically allocated storage that was allocated by instr */
void
instr_free(instr_t *instr)
{
    if (instr_is_label(instr) && instr_get_label_callback(instr) != NULL)
        (*instr->label_cb)(instr);
    if (TEST(INSTR_IS_NOALLOC_STRUCT, instr->flags))
        return;
    if (TEST(INSTR_RAW_BITS_ALLOCATED, instr->flags)) {
        instr_free_raw_bits(instr);
    }
    if (instr->num_dsts > 0) { /* checking num_dsts, not dsts, b/c of label data */
        heap_free(instr->dsts,
                  instr->num_dsts * sizeof(opnd_t) HEAPACCT(ACCT_IR));
        instr->dsts = NULL;
        instr->num_dsts = 0;
    }
    if (instr->num_srcs > 1) { /* checking num_src, not src, b/c of label data */
        /* remember one src is static, rest are dynamic */
        heap_free(instr->srcs,
                  (instr->num_srcs - 1) * sizeof(opnd_t) HEAPACCT(ACCT_IR));
        instr->srcs = NULL;
        instr->num_srcs = 0;
    }
}

/* zeroes out the fields of instr */
void
instr_init(instr_t *instr)
{
    /* everything initializes to 0, even flags, to indicate
     * an uninitialized instruction */
    memset((void *)instr, 0, sizeof(instr_t));
    instr_set_isa_mode(instr, dr_get_isa_mode());
}

/* Frees all dynamically allocated storage that was allocated by instr,
 * except for allocated raw bits.
 * Also zeroes out instr's fields, except for raw bit fields and next and prev
 * fields, whether instr is ok to mangle, and instr's x86 mode.
 * Use this routine when you want to decode more information into the
 * same instr_t structure.
 * This instr must have been initialized before!
 */
void
instr_reuse(instr_t *instr)
{
    byte *bits = NULL;
    uint len = 0;
    bool alloc = false;
    bool mangle = instr_is_app(instr);
    dr_isa_mode_t isa_mode = instr_get_isa_mode(instr);
    instr_t *next = instr->next;
    instr_t *prev = instr->prev;
    if (instr_raw_bits_valid(instr)) {
        if (instr_has_allocated_bits(instr)) {
            /* pretend has no allocated bits to prevent freeing of them */
            instr->flags &= ~INSTR_RAW_BITS_ALLOCATED;
            alloc = true;
        }
        bits = instr->bytes;
        len = instr->length;
    }
    instr_free(instr);
    instr_init(instr);
    /* now re-add them */
    instr->next = next;
    instr->prev = prev;
    if (bits != NULL) {
        instr->bytes = bits;
        instr->length = len;
        /* assume that the bits are now valid and the operands are not
         * (operand and eflags flags are already unset from init)
         */
        instr->flags |= INSTR_RAW_BITS_VALID;
        if (alloc)
            instr->flags |= INSTR_RAW_BITS_ALLOCATED;
    }
    /* preserve across the up-decode */
    instr_set_isa_mode(instr, isa_mode);
    if (!mangle)
        instr->flags |= INSTR_DO_NOT_MANGLE;
}

byte *
decode_common(byte *pc, byte *orig_pc, instr_t *instr);
uint
encode_common(byte *pc, instr_t *i, decode_info_t *di);

byte *
decode(byte *pc, instr_t *instr)
{
    return decode_common(pc, pc, instr);
}


/* If instr is not already fully decoded, decodes enough
 * from the raw bits pointed to by instr to bring it Level 3.
 * Assumes that instr is a single instr (i.e., NOT Level 0).
 */
void
instr_decode(instr_t *instr)
{
    if (!instr_operands_valid(instr)) {
        byte *next_pc;
        DEBUG_EXT_DECLARE(int old_len = instr->length;)
        /* decode() will use the current dcontext mode, but we want the instr mode */
        dr_isa_mode_t old_mode;
        dr_set_isa_mode(instr_get_isa_mode(instr), &old_mode);
        CLIENT_ASSERT(instr_raw_bits_valid(instr), "instr_decode: raw bits are invalid");
        instr_reuse(instr);
        next_pc = decode(instr_get_raw_bits(instr), instr);
        dr_set_isa_mode(old_mode, NULL);
        /* ok to be invalid, let caller deal with it */
        CLIENT_ASSERT(next_pc == NULL || (next_pc - instr->bytes == old_len),
                      "instr_decode requires a Level 1 or higher instruction");
    }
}

/* Calls instr_decode() with the current dcontext.  Mostly useful as the slow
 * path for IR routines that get inlined.
 */
NOINLINE /* rarely called */
    instr_t *
    instr_decode_with_current_dcontext(instr_t *instr)
{
    instr_decode(instr);
    return instr;
}

#define inlined_instr_get_opcode(instr)                                           \
    (IF_DEBUG_(CLIENT_ASSERT(sizeof(*instr) == sizeof(instr_t), "invalid type"))( \
        ((instr)->opcode == OP_UNDECODED)                                         \
            ? (instr_decode_with_current_dcontext(instr), (instr)->opcode)        \
            : (instr)->opcode))
int
instr_get_opcode(instr_t *instr)
{
    return inlined_instr_get_opcode(instr);
}

/* Returns true iff instr is a label meta-instruction */
bool
instr_is_label(instr_t *instr)
{
    return instr_opcode_valid(instr) && instr_get_opcode(instr) == OP_LABEL;
}

/** Triggers used for conditionally executed instructions. */
typedef enum _dr_pred_type_t {
    DR_PRED_NONE, /**< No predicate is present. */
#ifdef X86
    DR_PRED_O,   /**< x86 condition: overflow (OF=1). */
    DR_PRED_NO,  /**< x86 condition: no overflow (OF=0). */
    DR_PRED_B,   /**< x86 condition: below (CF=1). */
    DR_PRED_NB,  /**< x86 condition: not below (CF=0). */
    DR_PRED_Z,   /**< x86 condition: zero (ZF=1). */
    DR_PRED_NZ,  /**< x86 condition: not zero (ZF=0). */
    DR_PRED_BE,  /**< x86 condition: below or equal (CF=1 or ZF=1). */
    DR_PRED_NBE, /**< x86 condition: not below or equal (CF=0 and ZF=0). */
    DR_PRED_S,   /**< x86 condition: sign (SF=1). */
    DR_PRED_NS,  /**< x86 condition: not sign (SF=0). */
    DR_PRED_P,   /**< x86 condition: parity (PF=1). */
    DR_PRED_NP,  /**< x86 condition: not parity (PF=0). */
    DR_PRED_L,   /**< x86 condition: less (SF != OF). */
    DR_PRED_NL,  /**< x86 condition: not less (SF=OF). */
    DR_PRED_LE,  /**< x86 condition: less or equal (ZF=1 or SF != OF). */
    DR_PRED_NLE, /**< x86 condition: not less or equal (ZF=0 and SF=OF). */
    /**
     * x86 condition: special opcode-specific condition that depends on the
     * values of the source operands.  Thus, unlike all of the other conditions,
     * the source operands will be accessed even if the condition then fails
     * and the destinations are not touched.  Any written eflags are
     * unconditionally written, unlike regular destination operands.
     */
    DR_PRED_COMPLEX,
    /* Aliases for XINST_CREATE_jump_cond() and other cross-platform routines. */
    DR_PRED_EQ = DR_PRED_Z,  /**< Condition code: equal. */
    DR_PRED_NE = DR_PRED_NZ, /**< Condition code: not equal. */
    DR_PRED_LT = DR_PRED_L,  /**< Condition code: signed less than. */
    /* DR_PRED_LE already matches aarchxx */
    DR_PRED_GT = DR_PRED_NLE, /**< Condition code: signed greater than. */
    DR_PRED_GE = DR_PRED_NL,  /**< Condition code: signed greater than or equal. */
#endif
/* We resist using #elif here because otherwise doxygen will be unable to
 * document both defines, for X86 and for AARCHXX.
 */
#ifdef AARCHXX
    DR_PRED_EQ, /**< ARM condition: 0000 Equal                   (Z == 1)           */
    DR_PRED_NE, /**< ARM condition: 0001 Not equal               (Z == 0)           */
    DR_PRED_CS, /**< ARM condition: 0010 Carry set               (C == 1)           */
    DR_PRED_CC, /**< ARM condition: 0011 Carry clear             (C == 0)           */
    DR_PRED_MI, /**< ARM condition: 0100 Minus, negative         (N == 1)           */
    DR_PRED_PL, /**< ARM condition: 0101 Plus, positive or zero  (N == 0)           */
    DR_PRED_VS, /**< ARM condition: 0110 Overflow                (V == 1)           */
    DR_PRED_VC, /**< ARM condition: 0111 No overflow             (V == 0)           */
    DR_PRED_HI, /**< ARM condition: 1000 Unsigned higher         (C == 1 and Z == 0)*/
    DR_PRED_LS, /**< ARM condition: 1001 Unsigned lower or same  (C == 0 or Z == 1) */
    DR_PRED_GE, /**< ARM condition: 1010 Signed >=               (N == V)           */
    DR_PRED_LT, /**< ARM condition: 1011 Signed less than        (N != V)           */
    DR_PRED_GT, /**< ARM condition: 1100 Signed greater than     (Z == 0 and N == V)*/
    DR_PRED_LE, /**< ARM condition: 1101 Signed <=               (Z == 1 or N != V) */
    DR_PRED_AL, /**< ARM condition: 1110 Always (unconditional)                    */
#    ifdef AARCH64
    DR_PRED_NV, /**< ARM condition: 1111 Never, meaning always                     */
#    endif
#    ifdef ARM
    DR_PRED_OP, /**< ARM condition: 1111 Part of opcode                            */
#    endif
    /* Aliases */
    DR_PRED_HS = DR_PRED_CS, /**< ARM condition: alias for DR_PRED_CS. */
    DR_PRED_LO = DR_PRED_CC, /**< ARM condition: alias for DR_PRED_CC. */
#endif
} dr_pred_type_t;

#define PREFIX_PRED_BITS 5
#define PREFIX_PRED_BITPOS (32 - PREFIX_PRED_BITS)
#define PREFIX_PRED_MASK \
    (((1U << PREFIX_PRED_BITS) - 1) << PREFIX_PRED_BITPOS) /*0xf8000000 */

dr_pred_type_t
instr_get_predicate(instr_t *instr)
{
    /* Optimization: we assume prefixes are the high bits to avoid an & */
    return instr->prefixes >> PREFIX_PRED_BITPOS;
}

instr_t *
instr_set_predicate(instr_t *instr, dr_pred_type_t pred)
{
    instr->prefixes = ((instr->prefixes & ~PREFIX_PRED_MASK) |
                       ((pred << PREFIX_PRED_BITPOS) & PREFIX_PRED_MASK));
    return instr;
}

#    define EFLAGS_READ_N 0x00000001  /**< Reads N (negative flag). */
#    define EFLAGS_READ_Z 0x00000002  /**< Reads Z (zero flag). */
#    define EFLAGS_READ_C 0x00000004  /**< Reads C (carry flag). */
#    define EFLAGS_READ_V 0x00000008  /**< Reads V (overflow flag). */
#    define EFLAGS_READ_Q 0x00000010  /**< Reads Q (saturation flag). */
#    define EFLAGS_READ_GE 0x00000020 /**< Reads GE (>= for parallel arithmetic). */
#    define EFLAGS_READ_NZCV \
        (EFLAGS_READ_N | EFLAGS_READ_Z | EFLAGS_READ_C | EFLAGS_READ_V)
/** Platform-independent macro for reads all arithmetic flags. */
#    define EFLAGS_READ_ARITH (EFLAGS_READ_NZCV | EFLAGS_READ_Q | EFLAGS_READ_GE)
#    define EFLAGS_READ_ALL (EFLAGS_READ_ARITH) /**< Reads all flags. */
#    define EFLAGS_READ_NON_PRED                                         \
        EFLAGS_READ_GE                /**< Flags not read by predicates. \
                                       */
#    define EFLAGS_WRITE_N 0x00000040 /**< Reads N (negative). */
#    define EFLAGS_WRITE_Z 0x00000080 /**< Reads Z (zero). */
#    define EFLAGS_WRITE_C 0x00000100 /**< Reads C (carry). */
#    define EFLAGS_WRITE_V 0x00000200 /**< Reads V (overflow). */
#    define EFLAGS_WRITE_Q 0x00000400 /**< Reads Q (saturation). */
#    define EFLAGS_WRITE_GE                                    \
        0x00000800 /**< Reads GE (>= for parallel arithmetic). \
                    */
#    define EFLAGS_WRITE_NZCV \
        (EFLAGS_WRITE_N | EFLAGS_WRITE_Z | EFLAGS_WRITE_C | EFLAGS_WRITE_V)
/** Platform-independent macro for writes all arithmetic flags. */
#    define EFLAGS_WRITE_ARITH (EFLAGS_WRITE_NZCV | EFLAGS_WRITE_Q | EFLAGS_WRITE_GE)
#    define EFLAGS_WRITE_ALL (EFLAGS_WRITE_ARITH) /**< Writes all flags. */

/** Converts an EFLAGS_WRITE_* value to the corresponding EFLAGS_READ_* value. */
#    define EFLAGS_WRITE_TO_READ(x) ((x) >> 6)
/** Converts an EFLAGS_READ_* value to the corresponding EFLAGS_WRITE_* value. */
#    define EFLAGS_READ_TO_WRITE(x) ((x) << 6)

/**
 * The actual bits in the CPSR that we care about:\n<pre>
 *   31 30 29 28 27 ... 19 18 17 16 ... 5
 *    N  Z  C  V  Q       GE[3:0]       T </pre>
 */
enum {
    EFLAGS_N = 0x80000000,  /**< The bit in the CPSR register of N (negative flag). */
    EFLAGS_Z = 0x40000000,  /**< The bit in the CPSR register of Z (zero flag). */
    EFLAGS_C = 0x20000000,  /**< The bit in the CPSR register of C (carry flag). */
    EFLAGS_V = 0x10000000,  /**< The bit in the CPSR register of V (overflow flag). */
    EFLAGS_Q = 0x08000000,  /**< The bit in the CPSR register of Q (saturation flag). */
    EFLAGS_GE = 0x000f0000, /**< The bits in the CPSR register of GE[3:0]. */
    /** The bits in the CPSR register of N, Z, C, V, Q, and GE. */
    EFLAGS_ARITH = EFLAGS_N | EFLAGS_Z | EFLAGS_C | EFLAGS_V | EFLAGS_Q | EFLAGS_GE,
    /**
     * The bit in the CPSR register of T (Thumb mode indicator bit).  This is
     * not readable from user space and should only be examined when looking at
     * machine state from the kernel, such as in a signal handler.
     */
    EFLAGS_T = 0x00000020,
    /**
     * The bits in the CPSR register of the T32 IT block base condition.
     * This is not readable from user space and should only be examined when
     * looking at machine state from the kernel, such as in a signal handler.
     */
    EFLAGS_IT_COND = 0x0000e000,
    /**
     * The bits in the CPSR register of the T32 IT block size.
     * This is not readable from user space and should only be examined when
     * looking at machine state from the kernel, such as in a signal handler.
     */
    EFLAGS_IT_SIZE = 0x06001c00,
};

/** The bits in the CPSR register of the T32 IT block state. */
#    define EFLAGS_IT (EFLAGS_IT_COND | EFLAGS_IT_SIZE)

/** The bit in the 4-bit OP_msr immediate that selects the nzcvq status flags. */
#    define EFLAGS_MSR_NZCVQ 0x8
/** The bit in the 4-bit OP_msr immediate that selects the apsr_g status flags. */
#    define EFLAGS_MSR_G 0x4
/** The bits in the 4-bit OP_msr immediate that select the nzcvqg status flags. */
#    define EFLAGS_MSR_NZCVQG (EFLAGS_MSR_NZCVQ | EFLAGS_MSR_G)

/**
 * For AArch64 this struct holds features registers' values read by MRS instructions.
 * Used by proc_get_all_feature_bits().
 */
typedef struct {
    uint64 flags_aa64isar0; /**< AArch64 feature flags stored in ID_AA64ISAR0_EL1 */
    uint64 flags_aa64isar1; /**< AArch64 feature flags stored in ID_AA64ISAR1_EL1 */
    uint64 flags_aa64pfr0;  /**< AArch64 feature flags stored in ID_AA64PFR0_EL1 */
} features_t;
typedef enum { AA64ISAR0 = 0, AA64ISAR1 = 1, AA64PFR0 = 2 } feature_reg_idx_t;

/* On Arm, architectural features are defined and stored very differently from
 * X86. Specifically:
 * - There are multiple 64 bit system registers for features storage only, FREG.
 * - Each register is divided into nibbles representing a feature, NIBPOS.
 * - The value of a nibble represents a certain level of support for that feature, FVAL.
 * - The values can range from 0 to 15. In most cases 0 means a feature is not
 *   supported at all but in some cases 15 means a feature is not supported at
 *   all, NSFLAG.
 * The helper macro below packs feature data into 16 bits (ushort).
 */
#    define DEF_FEAT(FREG, NIBPOS, FVAL, NSFLAG) \
        ((ushort)((NSFLAG << 15) | (FREG << 8) | (NIBPOS << 4) | FVAL))
/**
 * Feature bits returned by cpuid for X86 and mrs for AArch64. Pass one of
 * these values to proc_has_feature() to determine whether the underlying
 * processor has the feature.
 */
typedef enum {
    /* Feature values returned in ID_AA64ISAR0_EL1 Instruction Set Attribute
     * Register 0
     */
    FEATURE_AESX = DEF_FEAT(AA64ISAR0, 1, 1, 0),     /**< AES<x> (AArch64) */
    FEATURE_PMULL = DEF_FEAT(AA64ISAR0, 1, 2, 0),    /**< PMULL/PMULL2 (AArch64) */
    FEATURE_SHA1 = DEF_FEAT(AA64ISAR0, 2, 1, 0),     /**< SHA1<x> (AArch64) */
    FEATURE_SHA256 = DEF_FEAT(AA64ISAR0, 3, 1, 0),   /**< SHA256<x> (AArch64) */
    FEATURE_SHA512 = DEF_FEAT(AA64ISAR0, 3, 2, 0),   /**< SHA512<x> (AArch64) */
    FEATURE_CRC32 = DEF_FEAT(AA64ISAR0, 4, 1, 0),    /**< CRC32<x> (AArch64) */
    FEATURE_LSE = DEF_FEAT(AA64ISAR0, 5, 2, 0),      /**< Atomic instructions (AArch64) */
    FEATURE_RDM = DEF_FEAT(AA64ISAR0, 7, 1, 0),      /**< SQRDMLAH,SQRDMLSH (AArch64) */
    FEATURE_SHA3 = DEF_FEAT(AA64ISAR0, 8, 1, 0),     /**< EOR3,RAX1,XAR,BCAX (AArch64) */
    FEATURE_SM3 = DEF_FEAT(AA64ISAR0, 9, 1, 0),      /**< SM3<x> (AArch64) */
    FEATURE_SM4 = DEF_FEAT(AA64ISAR0, 10, 1, 0),     /**< SM4E, SM4EKEY (AArch64) */
    FEATURE_DotProd = DEF_FEAT(AA64ISAR0, 11, 1, 0), /**< UDOT, SDOT (AArch64) */
    FEATURE_FHM = DEF_FEAT(AA64ISAR0, 12, 1, 0),     /**< FMLAL, FMLSL (AArch64) */
    FEATURE_FlagM = DEF_FEAT(AA64ISAR0, 13, 1, 0),   /**< CFINV,RMIF,SETF<x> (AArch64) */
    FEATURE_FlagM2 = DEF_FEAT(AA64ISAR0, 13, 2, 0),  /**< AXFLAG, XAFLAG (AArch64) */
    FEATURE_RNG = DEF_FEAT(AA64ISAR0, 15, 1, 0),     /**< RNDR, RNDRRS (AArch64) */
    /* FIXME i#5474: Define all FEATURE_s for ID_AA64ISAR1_EL1 and ID_AA64PFR0_EL1. */
    FEATURE_DPB = DEF_FEAT(AA64ISAR1, 0, 1, 0),  /**< DC CVAP (AArch64) */
    FEATURE_DPB2 = DEF_FEAT(AA64ISAR1, 0, 2, 0), /**< DC CVAP, DC CVADP (AArch64) */
    FEATURE_FP16 = DEF_FEAT(AA64PFR0, 4, 1, 1),  /**< Half-precision FP (AArch64) */
    FEATURE_SVE = DEF_FEAT(AA64PFR0, 8, 1, 1),   /**< Scalable Vectors (AArch64) */
} feature_bit_t;

#define GET_FEAT_REG(FEATURE) (feature_reg_idx_t)((((ushort)FEATURE) & 0x7F00) >> 8)
#define GET_FEAT_NIBPOS(FEATURE) ((((ushort)FEATURE) & 0x00F0) >> 4)
#define GET_FEAT_VAL(FEATURE) (((ushort)FEATURE) & 0x000F)
#define GET_FEAT_NSFLAG(FEATURE) ((((ushort)FEATURE) & 0x8000) >> 15)

/* Make sure to keep this in sync with proc_get_cache_size_str() in proc.c. */
/**
 * L1 and L2 cache sizes, used by proc_get_L1_icache_size(),
 * proc_get_L1_dcache_size(), proc_get_L2_cache_size(), and
 * proc_get_cache_size_str().
 */
typedef enum {
    CACHE_SIZE_8_KB,   /**< L1 or L2 cache size of 8 KB. */
    CACHE_SIZE_16_KB,  /**< L1 or L2 cache size of 16 KB. */
    CACHE_SIZE_32_KB,  /**< L1 or L2 cache size of 32 KB. */
    CACHE_SIZE_64_KB,  /**< L1 or L2 cache size of 64 KB. */
    CACHE_SIZE_128_KB, /**< L1 or L2 cache size of 128 KB. */
    CACHE_SIZE_256_KB, /**< L1 or L2 cache size of 256 KB. */
    CACHE_SIZE_512_KB, /**< L1 or L2 cache size of 512 KB. */
    CACHE_SIZE_1_MB,   /**< L1 or L2 cache size of 1 MB. */
    CACHE_SIZE_2_MB,   /**< L1 or L2 cache size of 2 MB. */
    CACHE_SIZE_UNKNOWN /**< Unknown L1 or L2 cache size. */
} cache_size_t;

/* information about a processor */
typedef struct _cpu_info_t {
    /* FIXME i#1551: x86 and arm use different description of cpu models
     * - x86: vendor, family, type, model, steeping,
     * - arm: implementer, architecture, variant, part, revision, model name, hardware.
     */
    uint vendor;
#ifdef AARCHXX
    uint architecture;
#endif
    uint family;
    uint type;
    uint model;
    uint stepping;
    uint L1_icache_size;
    uint L1_dcache_size;
    uint L2_cache_size;
    /* Feature bits in 4 32-bit values:
     * on X86:
     * - features in edx,
     * - features in ecx,
     * - extended features in edx, and
     * - extended features in ecx
     */
    features_t features;
    /* The brand string is a 48-character, null terminated string.
     * Declare as a 12-element uint so the compiler won't complain
     * when we store GPRs to it.  Initialization is "unknown" .
     */
    uint brand_string[12];
} cpu_info_t;

/** Constants returned by proc_get_vendor(). */
enum {
    VENDOR_INTEL,   /**< proc_get_vendor() processor identification: Intel */
    VENDOR_AMD,     /**< proc_get_vendor() processor identification: AMD */
    VENDOR_ARM,     /**< proc_get_vendor() processor identification: ARM */
    VENDOR_UNKNOWN, /**< proc_get_vendor() processor identification: unknown */
};

cpu_info_t cpu_info = { VENDOR_UNKNOWN,
#ifdef AARCHXX
                        0,
#endif
                        0,
                        0,
                        0,
                        0,
                        CACHE_SIZE_UNKNOWN,
                        CACHE_SIZE_UNKNOWN,
                        CACHE_SIZE_UNKNOWN,
#ifdef X86
                        { 0, 0, 0, 0 },
#elif defined(AARCHXX)
                        { 0, 0, 0 },
#endif
                        { 0x6e6b6e75, 0x006e776f } };

bool
proc_has_feature(feature_bit_t f)
{
#ifndef DR_HOST_NOT_TARGET
    /* Pretend features are supported for codec tests run on h/w which does not
     * support all features.
     */
#    if defined(BUILD_TESTS)
    if (f == FEATURE_LSE || f == FEATURE_RDM || f == FEATURE_FP16 ||
        f == FEATURE_DotProd || f == FEATURE_SVE)
        return true;
#    endif
    ushort feat_nibble, feat_val, freg_nibble, feat_nsflag;
    uint64 freg_val = 0;

    feature_reg_idx_t feat_reg = GET_FEAT_REG(f);
    if (feat_reg >= AA64ISAR0 && feat_reg <= AA64PFR0) {
        switch (feat_reg) {
        case AA64ISAR0: {
            freg_val = cpu_info.features.flags_aa64isar0;
            break;
        }
        case AA64ISAR1: {
            freg_val = cpu_info.features.flags_aa64isar1;
            break;
        }
        case AA64PFR0: {
            freg_val = cpu_info.features.flags_aa64pfr0;
            break;
        }
        default: CLIENT_ASSERT(false, "proc_has_feature: feature register index error");
        }
    } else {
        CLIENT_ASSERT(false, "proc_has_feature: invalid feature register");
    }

    /* Compare the nibble value in the feature register with the input
     * feature nibble value to establish if the feature set represented by the
     * nibble is supported. If the nibble value in the feature register is
     * 0 or 0xF, the feature is not supported at all
     */
    feat_nibble = GET_FEAT_NIBPOS(f);
    freg_nibble = (freg_val >> (feat_nibble * 4)) & 0xFULL;
    feat_nsflag = GET_FEAT_NSFLAG(f);
    if (feat_nsflag == 0 && freg_nibble == 0)
        return false;
    if (feat_nsflag == 1 && freg_nibble == 0xF)
        return false;

    feat_val = GET_FEAT_VAL(f);
    return (freg_nibble >= feat_val) ? true : false;
#else
    return false;
#endif
}

void
instr_set_eflags_valid(instr_t *instr, bool valid)
{
    if (valid) {
        instr->flags |= INSTR_EFLAGS_VALID;
        instr->flags |= INSTR_EFLAGS_6_VALID;
    } else {
        /* assume that arith flags are also invalid */
        instr->flags &= ~INSTR_EFLAGS_VALID;
        instr->flags &= ~INSTR_EFLAGS_6_VALID;
    }
}

bool
instr_valid(instr_t *instr)
{
    return (instr->opcode != OP_INVALID);
}

/* N.B.: this routine sets the "raw bits are valid" flag */
void
instr_set_raw_bits(instr_t *instr, byte *addr, uint length)
{
    if ((instr->flags & INSTR_RAW_BITS_ALLOCATED) != 0) {
        /* this does happen, when up-decoding an instr using its
         * own raw bits, so let it happen, but make sure allocated
         * bits aren't being lost
         */
        CLIENT_ASSERT(addr == instr->bytes && length == instr->length,
                      "instr_set_raw_bits: bits already there, but different");
    }
    if (!instr_valid(instr))
        instr_set_opcode(instr, OP_UNDECODED);
    instr->flags |= INSTR_RAW_BITS_VALID;
    instr->bytes = addr;
    instr->length = length;
}

instr_t *
instr_set_translation(instr_t *instr, app_pc addr)
{
    instr->translation = addr;
    return instr;
}

enum {
    NULL_kind,
    IMMED_INTEGER_kind,
    IMMED_FLOAT_kind,
    PC_kind,
    INSTR_kind,
    REG_kind,
    BASE_DISP_kind, /* optional DR_SEG_ reg + base reg + scaled index reg + disp */
    FAR_PC_kind,    /* a segment is specified as a selector value */
    FAR_INSTR_kind, /* a segment is specified as a selector value */
#    if defined(X64) || defined(ARM)
    REL_ADDR_kind, /* pc-relative address: ARM or 64-bit X86 only */
#    endif
#    ifdef X64
    ABS_ADDR_kind, /* 64-bit absolute address: x64 only */
#    endif
    MEM_INSTR_kind,
    IMMED_DOUBLE_kind,
    LAST_kind, /* sentinal; not a valid opnd kind */
};

#    define DOCHECK(level, statement) /* nothing */

opnd_t
opnd_create_immed_int(ptr_int_t i, opnd_size_t size)
{
    opnd_t opnd;
    opnd.kind = IMMED_INTEGER_kind;
    CLIENT_ASSERT(size < OPSZ_LAST_ENUM, "opnd_create_immed_int: invalid size");
    opnd.size = size;
    opnd.value.immed_int = i;
    opnd.aux.flags = 0;
    DOCHECK(1, { opnd_check_immed_size(i, size); });
    return opnd;
}

#define OPND_CREATE_INT32(val) opnd_create_immed_int((ptr_int_t)(val), OPSZ_4)
/**
 * Create an 8-byte immediate integer operand.
 * \note This is only relevant for x86: for ARM where immediate sizes are
 * ignored, simply use OPND_CREATE_INT().
 */
#    define OPND_CREATE_INT64(val) opnd_create_immed_int((ptr_int_t)(val), OPSZ_8)
/**
 * Create a pointer-sized immediate integer operand.
 * \note This is only relevant for x86: for ARM where immediate sizes are
 * ignored, simply use OPND_CREATE_INT().
 */
#    define OPND_CREATE_INTPTR OPND_CREATE_INT64
#define SYSLOG_INTERNAL_WARNING(...)


ptr_int_t
opnd_get_immed_int(opnd_t opnd)
{
    CLIENT_ASSERT(opnd_is_immed_int(opnd), "opnd_get_immed_int called on non-immed-int");
    return opnd.value.immed_int;
}

instr_t *
opnd_get_instr(opnd_t opnd)
{
    CLIENT_ASSERT(opnd_is_instr(opnd) || opnd_is_mem_instr(opnd),
                  "opnd_get_instr called on non-instr");
    return opnd.value.instr;
}

app_pc
opnd_get_pc(opnd_t opnd)
{
    return opnd.value.pc;
}

opnd_t
opnd_create_immed_uint(ptr_uint_t i, opnd_size_t size)
{
    opnd_t opnd;
    opnd.kind = IMMED_INTEGER_kind;
    CLIENT_ASSERT(size < OPSZ_LAST_ENUM, "opnd_create_immed_uint: invalid size");
    opnd.size = size;
    opnd.value.immed_int = (ptr_int_t)i;
    opnd.aux.flags = 0;
    DOCHECK(1, { opnd_check_immed_size(i, size); });
    return opnd;
}

/* We encode this enum plus the OPSZ_ extensions in bytes, so we can have
 * at most 256 total DR_REG_ plus OPSZ_ values.  Currently there are 165-odd.
 * Decoder assumes 32-bit, 16-bit, and 8-bit are in specific order
 * corresponding to modrm encodings.
 * We also assume that the DR_SEG_ constants are invalid as pointers for
 * our use in instr_info_t.code.
 * Also, reg_names array in encode.c corresponds to this enum order.
 * Plus, dr_reg_fixer array in encode.c.
 * Lots of optimizations assume same ordering of registers among
 * 32, 16, and 8  i.e. eax same position (first) in each etc.
 * reg_rm_selectable() assumes the GPR registers, mmx, and xmm are all in a row.
 */
/** Register identifiers. */
enum {
    /* The entire enum below overlaps with the OPSZ_ enum but all cases where the two are
     * used in the same field (instr_info_t operand sizes) have the type and distinguish
     * properly.
     * XXX i#3528: Switch from guaranteed-contiguous exposed enum ranges, which are not
     * possible to maintain long-term, to function interfaces.
     */
    DR_REG_NULL, /**< Sentinel value indicating no register, for address modes. */
#ifdef X86
    /* 64-bit general purpose */
    DR_REG_RAX, /**< The "rax" register. */
    DR_REG_RCX, /**< The "rcx" register. */
    DR_REG_RDX, /**< The "rdx" register. */
    DR_REG_RBX, /**< The "rbx" register. */
    DR_REG_RSP, /**< The "rsp" register. */
    DR_REG_RBP, /**< The "rbp" register. */
    DR_REG_RSI, /**< The "rsi" register. */
    DR_REG_RDI, /**< The "rdi" register. */
    DR_REG_R8,  /**< The "r8" register. */
    DR_REG_R9,  /**< The "r9" register. */
    DR_REG_R10, /**< The "r10" register. */
    DR_REG_R11, /**< The "r11" register. */
    DR_REG_R12, /**< The "r12" register. */
    DR_REG_R13, /**< The "r13" register. */
    DR_REG_R14, /**< The "r14" register. */
    DR_REG_R15, /**< The "r15" register. */
    /* 32-bit general purpose */
    DR_REG_EAX,  /**< The "eax" register. */
    DR_REG_ECX,  /**< The "ecx" register. */
    DR_REG_EDX,  /**< The "edx" register. */
    DR_REG_EBX,  /**< The "ebx" register. */
    DR_REG_ESP,  /**< The "esp" register. */
    DR_REG_EBP,  /**< The "ebp" register. */
    DR_REG_ESI,  /**< The "esi" register. */
    DR_REG_EDI,  /**< The "edi" register. */
    DR_REG_R8D,  /**< The "r8d" register. */
    DR_REG_R9D,  /**< The "r9d" register. */
    DR_REG_R10D, /**< The "r10d" register. */
    DR_REG_R11D, /**< The "r11d" register. */
    DR_REG_R12D, /**< The "r12d" register. */
    DR_REG_R13D, /**< The "r13d" register. */
    DR_REG_R14D, /**< The "r14d" register. */
    DR_REG_R15D, /**< The "r15d" register. */
    /* 16-bit general purpose */
    DR_REG_AX,   /**< The "ax" register. */
    DR_REG_CX,   /**< The "cx" register. */
    DR_REG_DX,   /**< The "dx" register. */
    DR_REG_BX,   /**< The "bx" register. */
    DR_REG_SP,   /**< The "sp" register. */
    DR_REG_BP,   /**< The "bp" register. */
    DR_REG_SI,   /**< The "si" register. */
    DR_REG_DI,   /**< The "di" register. */
    DR_REG_R8W,  /**< The "r8w" register. */
    DR_REG_R9W,  /**< The "r9w" register. */
    DR_REG_R10W, /**< The "r10w" register. */
    DR_REG_R11W, /**< The "r11w" register. */
    DR_REG_R12W, /**< The "r12w" register. */
    DR_REG_R13W, /**< The "r13w" register. */
    DR_REG_R14W, /**< The "r14w" register. */
    DR_REG_R15W, /**< The "r15w" register. */
    /* 8-bit general purpose */
    DR_REG_AL,   /**< The "al" register. */
    DR_REG_CL,   /**< The "cl" register. */
    DR_REG_DL,   /**< The "dl" register. */
    DR_REG_BL,   /**< The "bl" register. */
    DR_REG_AH,   /**< The "ah" register. */
    DR_REG_CH,   /**< The "ch" register. */
    DR_REG_DH,   /**< The "dh" register. */
    DR_REG_BH,   /**< The "bh" register. */
    DR_REG_R8L,  /**< The "r8l" register. */
    DR_REG_R9L,  /**< The "r9l" register. */
    DR_REG_R10L, /**< The "r10l" register. */
    DR_REG_R11L, /**< The "r11l" register. */
    DR_REG_R12L, /**< The "r12l" register. */
    DR_REG_R13L, /**< The "r13l" register. */
    DR_REG_R14L, /**< The "r14l" register. */
    DR_REG_R15L, /**< The "r15l" register. */
    DR_REG_SPL,  /**< The "spl" register. */
    DR_REG_BPL,  /**< The "bpl" register. */
    DR_REG_SIL,  /**< The "sil" register. */
    DR_REG_DIL,  /**< The "dil" register. */
    /* 64-BIT MMX */
    DR_REG_MM0, /**< The "mm0" register. */
    DR_REG_MM1, /**< The "mm1" register. */
    DR_REG_MM2, /**< The "mm2" register. */
    DR_REG_MM3, /**< The "mm3" register. */
    DR_REG_MM4, /**< The "mm4" register. */
    DR_REG_MM5, /**< The "mm5" register. */
    DR_REG_MM6, /**< The "mm6" register. */
    DR_REG_MM7, /**< The "mm7" register. */
    /* 128-BIT XMM */
    DR_REG_XMM0,  /**< The "xmm0" register. */
    DR_REG_XMM1,  /**< The "xmm1" register. */
    DR_REG_XMM2,  /**< The "xmm2" register. */
    DR_REG_XMM3,  /**< The "xmm3" register. */
    DR_REG_XMM4,  /**< The "xmm4" register. */
    DR_REG_XMM5,  /**< The "xmm5" register. */
    DR_REG_XMM6,  /**< The "xmm6" register. */
    DR_REG_XMM7,  /**< The "xmm7" register. */
    DR_REG_XMM8,  /**< The "xmm8" register. */
    DR_REG_XMM9,  /**< The "xmm9" register. */
    DR_REG_XMM10, /**< The "xmm10" register. */
    DR_REG_XMM11, /**< The "xmm11" register. */
    DR_REG_XMM12, /**< The "xmm12" register. */
    DR_REG_XMM13, /**< The "xmm13" register. */
    DR_REG_XMM14, /**< The "xmm14" register. */
    DR_REG_XMM15, /**< The "xmm15" register. */
    DR_REG_XMM16, /**< The "xmm16" register. */
    DR_REG_XMM17, /**< The "xmm17" register. */
    DR_REG_XMM18, /**< The "xmm18" register. */
    DR_REG_XMM19, /**< The "xmm19" register. */
    DR_REG_XMM20, /**< The "xmm20" register. */
    DR_REG_XMM21, /**< The "xmm21" register. */
    DR_REG_XMM22, /**< The "xmm22" register. */
    DR_REG_XMM23, /**< The "xmm23" register. */
    DR_REG_XMM24, /**< The "xmm24" register. */
    DR_REG_XMM25, /**< The "xmm25" register. */
    DR_REG_XMM26, /**< The "xmm26" register. */
    DR_REG_XMM27, /**< The "xmm27" register. */
    DR_REG_XMM28, /**< The "xmm28" register. */
    DR_REG_XMM29, /**< The "xmm29" register. */
    DR_REG_XMM30, /**< The "xmm30" register. */
    DR_REG_XMM31, /**< The "xmm31" register. */
    /* 32 enums are reserved for future Intel SIMD extensions. */
    RESERVED_XMM = DR_REG_XMM31 + 32,
    /* floating point registers */
    DR_REG_ST0, /**< The "st0" register. */
    DR_REG_ST1, /**< The "st1" register. */
    DR_REG_ST2, /**< The "st2" register. */
    DR_REG_ST3, /**< The "st3" register. */
    DR_REG_ST4, /**< The "st4" register. */
    DR_REG_ST5, /**< The "st5" register. */
    DR_REG_ST6, /**< The "st6" register. */
    DR_REG_ST7, /**< The "st7" register. */
    /* segments (order from "Sreg" description in Intel manual) */
    DR_SEG_ES, /**< The "es" register. */
    DR_SEG_CS, /**< The "cs" register. */
    DR_SEG_SS, /**< The "ss" register. */
    DR_SEG_DS, /**< The "ds" register. */
    DR_SEG_FS, /**< The "fs" register. */
    DR_SEG_GS, /**< The "gs" register. */
    /* debug & control registers (privileged access only; 8-15 for future processors)
     */
    DR_REG_DR0,  /**< The "dr0" register. */
    DR_REG_DR1,  /**< The "dr1" register. */
    DR_REG_DR2,  /**< The "dr2" register. */
    DR_REG_DR3,  /**< The "dr3" register. */
    DR_REG_DR4,  /**< The "dr4" register. */
    DR_REG_DR5,  /**< The "dr5" register. */
    DR_REG_DR6,  /**< The "dr6" register. */
    DR_REG_DR7,  /**< The "dr7" register. */
    DR_REG_DR8,  /**< The "dr8" register. */
    DR_REG_DR9,  /**< The "dr9" register. */
    DR_REG_DR10, /**< The "dr10" register. */
    DR_REG_DR11, /**< The "dr11" register. */
    DR_REG_DR12, /**< The "dr12" register. */
    DR_REG_DR13, /**< The "dr13" register. */
    DR_REG_DR14, /**< The "dr14" register. */
    DR_REG_DR15, /**< The "dr15" register. */
    /* cr9-cr15 do not yet exist on current x64 hardware */
    DR_REG_CR0,  /**< The "cr0" register. */
    DR_REG_CR1,  /**< The "cr1" register. */
    DR_REG_CR2,  /**< The "cr2" register. */
    DR_REG_CR3,  /**< The "cr3" register. */
    DR_REG_CR4,  /**< The "cr4" register. */
    DR_REG_CR5,  /**< The "cr5" register. */
    DR_REG_CR6,  /**< The "cr6" register. */
    DR_REG_CR7,  /**< The "cr7" register. */
    DR_REG_CR8,  /**< The "cr8" register. */
    DR_REG_CR9,  /**< The "cr9" register. */
    DR_REG_CR10, /**< The "cr10" register. */
    DR_REG_CR11, /**< The "cr11" register. */
    DR_REG_CR12, /**< The "cr12" register. */
    DR_REG_CR13, /**< The "cr13" register. */
    DR_REG_CR14, /**< The "cr14" register. */
    DR_REG_CR15, /**< The "cr15" register. */
    /* All registers above this point may be used as opnd_size_t and therefore
     * need to fit into a byte (checked in d_r_arch_init()). Register enums
     * below this point must not be used as opnd_size_t.
     */
    DR_REG_MAX_AS_OPSZ = DR_REG_CR15, /**< The "cr15" register. */
    DR_REG_INVALID, /**< Sentinel value indicating an invalid register. */
    /* 256-BIT YMM */
    DR_REG_YMM0,  /**< The "ymm0" register. */
    DR_REG_YMM1,  /**< The "ymm1" register. */
    DR_REG_YMM2,  /**< The "ymm2" register. */
    DR_REG_YMM3,  /**< The "ymm3" register. */
    DR_REG_YMM4,  /**< The "ymm4" register. */
    DR_REG_YMM5,  /**< The "ymm5" register. */
    DR_REG_YMM6,  /**< The "ymm6" register. */
    DR_REG_YMM7,  /**< The "ymm7" register. */
    DR_REG_YMM8,  /**< The "ymm8" register. */
    DR_REG_YMM9,  /**< The "ymm9" register. */
    DR_REG_YMM10, /**< The "ymm10" register. */
    DR_REG_YMM11, /**< The "ymm11" register. */
    DR_REG_YMM12, /**< The "ymm12" register. */
    DR_REG_YMM13, /**< The "ymm13" register. */
    DR_REG_YMM14, /**< The "ymm14" register. */
    DR_REG_YMM15, /**< The "ymm15" register. */
    DR_REG_YMM16, /**< The "ymm16" register. */
    DR_REG_YMM17, /**< The "ymm17" register. */
    DR_REG_YMM18, /**< The "ymm18" register. */
    DR_REG_YMM19, /**< The "ymm19" register. */
    DR_REG_YMM20, /**< The "ymm20" register. */
    DR_REG_YMM21, /**< The "ymm21" register. */
    DR_REG_YMM22, /**< The "ymm22" register. */
    DR_REG_YMM23, /**< The "ymm23" register. */
    DR_REG_YMM24, /**< The "ymm24" register. */
    DR_REG_YMM25, /**< The "ymm25" register. */
    DR_REG_YMM26, /**< The "ymm26" register. */
    DR_REG_YMM27, /**< The "ymm27" register. */
    DR_REG_YMM28, /**< The "ymm28" register. */
    DR_REG_YMM29, /**< The "ymm29" register. */
    DR_REG_YMM30, /**< The "ymm30" register. */
    DR_REG_YMM31, /**< The "ymm31" register. */
    /* 32 enums are reserved for future Intel SIMD extensions. */
    RESERVED_YMM = DR_REG_YMM31 + 32,
    /* 512-BIT ZMM */
    DR_REG_ZMM0,  /**< The "zmm0" register. */
    DR_REG_ZMM1,  /**< The "zmm1" register. */
    DR_REG_ZMM2,  /**< The "zmm2" register. */
    DR_REG_ZMM3,  /**< The "zmm3" register. */
    DR_REG_ZMM4,  /**< The "zmm4" register. */
    DR_REG_ZMM5,  /**< The "zmm5" register. */
    DR_REG_ZMM6,  /**< The "zmm6" register. */
    DR_REG_ZMM7,  /**< The "zmm7" register. */
    DR_REG_ZMM8,  /**< The "zmm8" register. */
    DR_REG_ZMM9,  /**< The "zmm9" register. */
    DR_REG_ZMM10, /**< The "zmm10" register. */
    DR_REG_ZMM11, /**< The "zmm11" register. */
    DR_REG_ZMM12, /**< The "zmm12" register. */
    DR_REG_ZMM13, /**< The "zmm13" register. */
    DR_REG_ZMM14, /**< The "zmm14" register. */
    DR_REG_ZMM15, /**< The "zmm15" register. */
    DR_REG_ZMM16, /**< The "zmm16" register. */
    DR_REG_ZMM17, /**< The "zmm17" register. */
    DR_REG_ZMM18, /**< The "zmm18" register. */
    DR_REG_ZMM19, /**< The "zmm19" register. */
    DR_REG_ZMM20, /**< The "zmm20" register. */
    DR_REG_ZMM21, /**< The "zmm21" register. */
    DR_REG_ZMM22, /**< The "zmm22" register. */
    DR_REG_ZMM23, /**< The "zmm23" register. */
    DR_REG_ZMM24, /**< The "zmm24" register. */
    DR_REG_ZMM25, /**< The "zmm25" register. */
    DR_REG_ZMM26, /**< The "zmm26" register. */
    DR_REG_ZMM27, /**< The "zmm27" register. */
    DR_REG_ZMM28, /**< The "zmm28" register. */
    DR_REG_ZMM29, /**< The "zmm29" register. */
    DR_REG_ZMM30, /**< The "zmm30" register. */
    DR_REG_ZMM31, /**< The "zmm31" register. */
    /* 32 enums are reserved for future Intel SIMD extensions. */
    RESERVED_ZMM = DR_REG_ZMM31 + 32,
    /* opmask registers */
    DR_REG_K0, /**< The "k0" register. */
    DR_REG_K1, /**< The "k1" register. */
    DR_REG_K2, /**< The "k2" register. */
    DR_REG_K3, /**< The "k3" register. */
    DR_REG_K4, /**< The "k4" register. */
    DR_REG_K5, /**< The "k5" register. */
    DR_REG_K6, /**< The "k6" register. */
    DR_REG_K7, /**< The "k7" register. */
    /* 8 enums are reserved for future Intel SIMD mask extensions. */
    RESERVED_OPMASK = DR_REG_K7 + 8,
    /* Bounds registers for MPX. */
    DR_REG_BND0, /**< The "bnd0" register. */
    DR_REG_BND1, /**< The "bnd1" register. */
    DR_REG_BND2, /**< The "bnd2" register. */
    DR_REG_BND3, /**< The "bnd3" register. */

/****************************************************************************/
#elif defined(AARCHXX)
    DR_REG_INVALID, /**< Sentinel value indicating an invalid register. */

#    ifdef AARCH64
    /* 64-bit general purpose */
    DR_REG_X0,  /**< The "x0" register. */
    DR_REG_X1,  /**< The "x1" register. */
    DR_REG_X2,  /**< The "x2" register. */
    DR_REG_X3,  /**< The "x3" register. */
    DR_REG_X4,  /**< The "x4" register. */
    DR_REG_X5,  /**< The "x5" register. */
    DR_REG_X6,  /**< The "x6" register. */
    DR_REG_X7,  /**< The "x7" register. */
    DR_REG_X8,  /**< The "x8" register. */
    DR_REG_X9,  /**< The "x9" register. */
    DR_REG_X10, /**< The "x10" register. */
    DR_REG_X11, /**< The "x11" register. */
    DR_REG_X12, /**< The "x12" register. */
    DR_REG_X13, /**< The "x13" register. */
    DR_REG_X14, /**< The "x14" register. */
    DR_REG_X15, /**< The "x15" register. */
    DR_REG_X16, /**< The "x16" register. */
    DR_REG_X17, /**< The "x17" register. */
    DR_REG_X18, /**< The "x18" register. */
    DR_REG_X19, /**< The "x19" register. */
    DR_REG_X20, /**< The "x20" register. */
    DR_REG_X21, /**< The "x21" register. */
    DR_REG_X22, /**< The "x22" register. */
    DR_REG_X23, /**< The "x23" register. */
    DR_REG_X24, /**< The "x24" register. */
    DR_REG_X25, /**< The "x25" register. */
    DR_REG_X26, /**< The "x26" register. */
    DR_REG_X27, /**< The "x27" register. */
    DR_REG_X28, /**< The "x28" register. */
    DR_REG_X29, /**< The "x29" register. */
    DR_REG_X30, /**< The "x30" register. */
    DR_REG_XSP, /**< The "xsp" stack pointer register. */
    DR_REG_XZR, /**< The "xzr" zero pseudo-register; not included in GPRs. */

    /* 32-bit general purpose */
    DR_REG_W0,  /**< The "w0" register. */
    DR_REG_W1,  /**< The "w1" register. */
    DR_REG_W2,  /**< The "w2" register. */
    DR_REG_W3,  /**< The "w3" register. */
    DR_REG_W4,  /**< The "w4" register. */
    DR_REG_W5,  /**< The "w5" register. */
    DR_REG_W6,  /**< The "w6" register. */
    DR_REG_W7,  /**< The "w7" register. */
    DR_REG_W8,  /**< The "w8" register. */
    DR_REG_W9,  /**< The "w9" register. */
    DR_REG_W10, /**< The "w10" register. */
    DR_REG_W11, /**< The "w11" register. */
    DR_REG_W12, /**< The "w12" register. */
    DR_REG_W13, /**< The "w13" register. */
    DR_REG_W14, /**< The "w14" register. */
    DR_REG_W15, /**< The "w15" register. */
    DR_REG_W16, /**< The "w16" register. */
    DR_REG_W17, /**< The "w17" register. */
    DR_REG_W18, /**< The "w18" register. */
    DR_REG_W19, /**< The "w19" register. */
    DR_REG_W20, /**< The "w20" register. */
    DR_REG_W21, /**< The "w21" register. */
    DR_REG_W22, /**< The "w22" register. */
    DR_REG_W23, /**< The "w23" register. */
    DR_REG_W24, /**< The "w24" register. */
    DR_REG_W25, /**< The "w25" register. */
    DR_REG_W26, /**< The "w26" register. */
    DR_REG_W27, /**< The "w27" register. */
    DR_REG_W28, /**< The "w28" register. */
    DR_REG_W29, /**< The "w29" register. */
    DR_REG_W30, /**< The "w30" register. */
    DR_REG_WSP, /**< The "wsp" bottom half of the stack pointer register. */
    DR_REG_WZR, /**< The "wzr" zero pseudo-register. */
#    else
    /* 32-bit general purpose */
    DR_REG_R0,                                /**< The "r0" register. */
    DR_REG_R1,                                /**< The "r1" register. */
    DR_REG_R2,                                /**< The "r2" register. */
    DR_REG_R3,                                /**< The "r3" register. */
    DR_REG_R4,                                /**< The "r4" register. */
    DR_REG_R5,                                /**< The "r5" register. */
    DR_REG_R6,                                /**< The "r6" register. */
    DR_REG_R7,                                /**< The "r7" register. */
    DR_REG_R8,                                /**< The "r8" register. */
    DR_REG_R9,                                /**< The "r9" register. */
    DR_REG_R10,                               /**< The "r10" register. */
    DR_REG_R11,                               /**< The "r11" register. */
    DR_REG_R12,                               /**< The "r12" register. */
    DR_REG_R13,                               /**< The "r13" register. */
    DR_REG_R14,                               /**< The "r14" register. */
    DR_REG_R15,                               /**< The "r15" register. */
#    endif

    /* 128-bit SIMD registers */
    DR_REG_Q0,  /**< The "q0" register. */
    DR_REG_Q1,  /**< The "q1" register. */
    DR_REG_Q2,  /**< The "q2" register. */
    DR_REG_Q3,  /**< The "q3" register. */
    DR_REG_Q4,  /**< The "q4" register. */
    DR_REG_Q5,  /**< The "q5" register. */
    DR_REG_Q6,  /**< The "q6" register. */
    DR_REG_Q7,  /**< The "q7" register. */
    DR_REG_Q8,  /**< The "q8" register. */
    DR_REG_Q9,  /**< The "q9" register. */
    DR_REG_Q10, /**< The "q10" register. */
    DR_REG_Q11, /**< The "q11" register. */
    DR_REG_Q12, /**< The "q12" register. */
    DR_REG_Q13, /**< The "q13" register. */
    DR_REG_Q14, /**< The "q14" register. */
    DR_REG_Q15, /**< The "q15" register. */
    /* x64-only but simpler code to not ifdef it */
    DR_REG_Q16, /**< The "q16" register. */
    DR_REG_Q17, /**< The "q17" register. */
    DR_REG_Q18, /**< The "q18" register. */
    DR_REG_Q19, /**< The "q19" register. */
    DR_REG_Q20, /**< The "q20" register. */
    DR_REG_Q21, /**< The "q21" register. */
    DR_REG_Q22, /**< The "q22" register. */
    DR_REG_Q23, /**< The "q23" register. */
    DR_REG_Q24, /**< The "q24" register. */
    DR_REG_Q25, /**< The "q25" register. */
    DR_REG_Q26, /**< The "q26" register. */
    DR_REG_Q27, /**< The "q27" register. */
    DR_REG_Q28, /**< The "q28" register. */
    DR_REG_Q29, /**< The "q29" register. */
    DR_REG_Q30, /**< The "q30" register. */
    DR_REG_Q31, /**< The "q31" register. */
    /* 64-bit SIMD registers */
    DR_REG_D0,  /**< The "d0" register. */
    DR_REG_D1,  /**< The "d1" register. */
    DR_REG_D2,  /**< The "d2" register. */
    DR_REG_D3,  /**< The "d3" register. */
    DR_REG_D4,  /**< The "d4" register. */
    DR_REG_D5,  /**< The "d5" register. */
    DR_REG_D6,  /**< The "d6" register. */
    DR_REG_D7,  /**< The "d7" register. */
    DR_REG_D8,  /**< The "d8" register. */
    DR_REG_D9,  /**< The "d9" register. */
    DR_REG_D10, /**< The "d10" register. */
    DR_REG_D11, /**< The "d11" register. */
    DR_REG_D12, /**< The "d12" register. */
    DR_REG_D13, /**< The "d13" register. */
    DR_REG_D14, /**< The "d14" register. */
    DR_REG_D15, /**< The "d15" register. */
    DR_REG_D16, /**< The "d16" register. */
    DR_REG_D17, /**< The "d17" register. */
    DR_REG_D18, /**< The "d18" register. */
    DR_REG_D19, /**< The "d19" register. */
    DR_REG_D20, /**< The "d20" register. */
    DR_REG_D21, /**< The "d21" register. */
    DR_REG_D22, /**< The "d22" register. */
    DR_REG_D23, /**< The "d23" register. */
    DR_REG_D24, /**< The "d24" register. */
    DR_REG_D25, /**< The "d25" register. */
    DR_REG_D26, /**< The "d26" register. */
    DR_REG_D27, /**< The "d27" register. */
    DR_REG_D28, /**< The "d28" register. */
    DR_REG_D29, /**< The "d29" register. */
    DR_REG_D30, /**< The "d30" register. */
    DR_REG_D31, /**< The "d31" register. */
    /* 32-bit SIMD registers */
    DR_REG_S0,  /**< The "s0" register. */
    DR_REG_S1,  /**< The "s1" register. */
    DR_REG_S2,  /**< The "s2" register. */
    DR_REG_S3,  /**< The "s3" register. */
    DR_REG_S4,  /**< The "s4" register. */
    DR_REG_S5,  /**< The "s5" register. */
    DR_REG_S6,  /**< The "s6" register. */
    DR_REG_S7,  /**< The "s7" register. */
    DR_REG_S8,  /**< The "s8" register. */
    DR_REG_S9,  /**< The "s9" register. */
    DR_REG_S10, /**< The "s10" register. */
    DR_REG_S11, /**< The "s11" register. */
    DR_REG_S12, /**< The "s12" register. */
    DR_REG_S13, /**< The "s13" register. */
    DR_REG_S14, /**< The "s14" register. */
    DR_REG_S15, /**< The "s15" register. */
    DR_REG_S16, /**< The "s16" register. */
    DR_REG_S17, /**< The "s17" register. */
    DR_REG_S18, /**< The "s18" register. */
    DR_REG_S19, /**< The "s19" register. */
    DR_REG_S20, /**< The "s20" register. */
    DR_REG_S21, /**< The "s21" register. */
    DR_REG_S22, /**< The "s22" register. */
    DR_REG_S23, /**< The "s23" register. */
    DR_REG_S24, /**< The "s24" register. */
    DR_REG_S25, /**< The "s25" register. */
    DR_REG_S26, /**< The "s26" register. */
    DR_REG_S27, /**< The "s27" register. */
    DR_REG_S28, /**< The "s28" register. */
    DR_REG_S29, /**< The "s29" register. */
    DR_REG_S30, /**< The "s30" register. */
    DR_REG_S31, /**< The "s31" register. */
    /* 16-bit SIMD registers */
    DR_REG_H0,  /**< The "h0" register. */
    DR_REG_H1,  /**< The "h1" register. */
    DR_REG_H2,  /**< The "h2" register. */
    DR_REG_H3,  /**< The "h3" register. */
    DR_REG_H4,  /**< The "h4" register. */
    DR_REG_H5,  /**< The "h5" register. */
    DR_REG_H6,  /**< The "h6" register. */
    DR_REG_H7,  /**< The "h7" register. */
    DR_REG_H8,  /**< The "h8" register. */
    DR_REG_H9,  /**< The "h9" register. */
    DR_REG_H10, /**< The "h10" register. */
    DR_REG_H11, /**< The "h11" register. */
    DR_REG_H12, /**< The "h12" register. */
    DR_REG_H13, /**< The "h13" register. */
    DR_REG_H14, /**< The "h14" register. */
    DR_REG_H15, /**< The "h15" register. */
    DR_REG_H16, /**< The "h16" register. */
    DR_REG_H17, /**< The "h17" register. */
    DR_REG_H18, /**< The "h18" register. */
    DR_REG_H19, /**< The "h19" register. */
    DR_REG_H20, /**< The "h20" register. */
    DR_REG_H21, /**< The "h21" register. */
    DR_REG_H22, /**< The "h22" register. */
    DR_REG_H23, /**< The "h23" register. */
    DR_REG_H24, /**< The "h24" register. */
    DR_REG_H25, /**< The "h25" register. */
    DR_REG_H26, /**< The "h26" register. */
    DR_REG_H27, /**< The "h27" register. */
    DR_REG_H28, /**< The "h28" register. */
    DR_REG_H29, /**< The "h29" register. */
    DR_REG_H30, /**< The "h30" register. */
    DR_REG_H31, /**< The "h31" register. */
    /* 8-bit SIMD registers */
    DR_REG_B0,  /**< The "b0" register. */
    DR_REG_B1,  /**< The "b1" register. */
    DR_REG_B2,  /**< The "b2" register. */
    DR_REG_B3,  /**< The "b3" register. */
    DR_REG_B4,  /**< The "b4" register. */
    DR_REG_B5,  /**< The "b5" register. */
    DR_REG_B6,  /**< The "b6" register. */
    DR_REG_B7,  /**< The "b7" register. */
    DR_REG_B8,  /**< The "b8" register. */
    DR_REG_B9,  /**< The "b9" register. */
    DR_REG_B10, /**< The "b10" register. */
    DR_REG_B11, /**< The "b11" register. */
    DR_REG_B12, /**< The "b12" register. */
    DR_REG_B13, /**< The "b13" register. */
    DR_REG_B14, /**< The "b14" register. */
    DR_REG_B15, /**< The "b15" register. */
    DR_REG_B16, /**< The "b16" register. */
    DR_REG_B17, /**< The "b17" register. */
    DR_REG_B18, /**< The "b18" register. */
    DR_REG_B19, /**< The "b19" register. */
    DR_REG_B20, /**< The "b20" register. */
    DR_REG_B21, /**< The "b21" register. */
    DR_REG_B22, /**< The "b22" register. */
    DR_REG_B23, /**< The "b23" register. */
    DR_REG_B24, /**< The "b24" register. */
    DR_REG_B25, /**< The "b25" register. */
    DR_REG_B26, /**< The "b26" register. */
    DR_REG_B27, /**< The "b27" register. */
    DR_REG_B28, /**< The "b28" register. */
    DR_REG_B29, /**< The "b29" register. */
    DR_REG_B30, /**< The "b30" register. */
    DR_REG_B31, /**< The "b31" register. */

#    ifndef AARCH64
    /* Coprocessor registers */
    DR_REG_CR0,  /**< The "cr0" register. */
    DR_REG_CR1,  /**< The "cr1" register. */
    DR_REG_CR2,  /**< The "cr2" register. */
    DR_REG_CR3,  /**< The "cr3" register. */
    DR_REG_CR4,  /**< The "cr4" register. */
    DR_REG_CR5,  /**< The "cr5" register. */
    DR_REG_CR6,  /**< The "cr6" register. */
    DR_REG_CR7,  /**< The "cr7" register. */
    DR_REG_CR8,  /**< The "cr8" register. */
    DR_REG_CR9,  /**< The "cr9" register. */
    DR_REG_CR10, /**< The "cr10" register. */
    DR_REG_CR11, /**< The "cr11" register. */
    DR_REG_CR12, /**< The "cr12" register. */
    DR_REG_CR13, /**< The "cr13" register. */
    DR_REG_CR14, /**< The "cr14" register. */
    DR_REG_CR15, /**< The "cr15" register. */
#    endif

/* We decided against DR_REG_RN_TH (top half), DR_REG_RN_BH (bottom half
 * for 32-bit as we have the W versions for 64-bit), and DR_REG_RN_BB
 * (bottom byte) as they are not available in the ISA and which portion
 * of a GPR is selected purely by the opcode.  Our decoder will create
 * a partial register for these to help tools, but it won't specify which
 * part of the register.
 */

/* Though on x86 we don't list eflags for even things like popf that write
 * other bits beyond aflags, here we do explicitly list cpsr and spsr for
 * OP_mrs and OP_msr to distinguish them and make things clearer.
 */
#    ifdef AARCH64
    DR_REG_NZCV,            /**< The "nzcv" register. */
    DR_REG_FPCR,            /**< The "fpcr" register. */
    DR_REG_FPSR,            /**< The "fpsr" register. */
    DR_REG_MDCCSR_EL0,      /**< The "mdccsr_el0" register. */
    DR_REG_DBGDTR_EL0,      /**< The "dbgdtr_el0" register. */
    DR_REG_DBGDTRRX_EL0,    /**< The "dbgdtrrx_el0" register. */
    DR_REG_SP_EL0,          /**< The "sp_el0" register. */
    DR_REG_SPSEL,           /**< The "spsel" register. */
    DR_REG_DAIFSET,         /**< The "DAIFSet" register. */
    DR_REG_DAIFCLR,         /**< The "DAIFClr" register. */
    DR_REG_CURRENTEL,       /**< The "currentel" register. */
    DR_REG_PAN,             /**< The "pan" register. */
    DR_REG_UAO,             /**< The "uao" register. */
    DR_REG_CTR_EL0,         /**< The "ctr_el0" register. */
    DR_REG_DCZID_EL0,       /**< The "dczid_el0" register. */
    DR_REG_RNDR,            /**< The "rndr" register. */
    DR_REG_RNDRRS,          /**< The "rndrrs" register. */
    DR_REG_DAIF,            /**< The "daif" register. */
    DR_REG_DIT,             /**< The "dit" register. */
    DR_REG_SSBS,            /**< The "ssbs" register. */
    DR_REG_TCO,             /**< The "tco" register. */
    DR_REG_DSPSR_EL0,       /**< The "dspsr_el0" register. */
    DR_REG_DLR_EL0,         /**< The "dlr_el0" register. */
    DR_REG_PMCR_EL0,        /**< The "pmcr_el0" register. */
    DR_REG_PMCNTENSET_EL0,  /**< The "pmcntenset_el0" register. */
    DR_REG_PMCNTENCLR_EL0,  /**< The "pmcntenclr_el0" register. */
    DR_REG_PMOVSCLR_EL0,    /**< The "pmovsclr_el0" register. */
    DR_REG_PMSWINC_EL0,     /**< The "pmswinc_el0" register. */
    DR_REG_PMSELR_EL0,      /**< The "pmselr_el0" register. */
    DR_REG_PMCEID0_EL0,     /**< The "pmceid0_el0" register. */
    DR_REG_PMCEID1_EL0,     /**< The "pmceid1_el0" register. */
    DR_REG_PMCCNTR_EL0,     /**< The "pmccntr_el0" register. */
    DR_REG_PMXEVTYPER_EL0,  /**< The "pmxevtyper_el0" register. */
    DR_REG_PMXEVCNTR_EL0,   /**< The "pmxevcntr_el0" register. */
    DR_REG_PMUSERENR_EL0,   /**< The "pmuserenr_el0" register. */
    DR_REG_PMOVSSET_EL0,    /**< The "pmovsset_el0" register. */
    DR_REG_SCXTNUM_EL0,     /**< The "scxtnum_el0" register. */
    DR_REG_CNTFRQ_EL0,      /**< The "cntfrq_el0" register. */
    DR_REG_CNTPCT_EL0,      /**< The "cntpct_el0" register. */
    DR_REG_CNTP_TVAL_EL0,   /**< The "cntp_tval_el0" register. */
    DR_REG_CNTP_CTL_EL0,    /**< The "cntp_ctl_el0" register. */
    DR_REG_CNTP_CVAL_EL0,   /**< The "cntp_cval_el0" register. */
    DR_REG_CNTV_TVAL_EL0,   /**< The "cntv_tval_el0" register. */
    DR_REG_CNTV_CTL_EL0,    /**< The "cntv_ctl_el0" register. */
    DR_REG_CNTV_CVAL_EL0,   /**< The "cntv_cval_el0" register. */
    DR_REG_PMEVCNTR0_EL0,   /**< The "pmevcntr0_el0" register. */
    DR_REG_PMEVCNTR1_EL0,   /**< The "pmevcntr1_el0" register. */
    DR_REG_PMEVCNTR2_EL0,   /**< The "pmevcntr2_el0" register. */
    DR_REG_PMEVCNTR3_EL0,   /**< The "pmevcntr3_el0" register. */
    DR_REG_PMEVCNTR4_EL0,   /**< The "pmevcntr4_el0" register. */
    DR_REG_PMEVCNTR5_EL0,   /**< The "pmevcntr5_el0" register. */
    DR_REG_PMEVCNTR6_EL0,   /**< The "pmevcntr6_el0" register. */
    DR_REG_PMEVCNTR7_EL0,   /**< The "pmevcntr7_el0" register. */
    DR_REG_PMEVCNTR8_EL0,   /**< The "pmevcntr8_el0" register. */
    DR_REG_PMEVCNTR9_EL0,   /**< The "pmevcntr9_el0" register. */
    DR_REG_PMEVCNTR10_EL0,  /**< The "pmevcntr10_el0" register. */
    DR_REG_PMEVCNTR11_EL0,  /**< The "pmevcntr11_el0" register. */
    DR_REG_PMEVCNTR12_EL0,  /**< The "pmevcntr12_el0" register. */
    DR_REG_PMEVCNTR13_EL0,  /**< The "pmevcntr13_el0" register. */
    DR_REG_PMEVCNTR14_EL0,  /**< The "pmevcntr14_el0" register. */
    DR_REG_PMEVCNTR15_EL0,  /**< The "pmevcntr15_el0" register. */
    DR_REG_PMEVCNTR16_EL0,  /**< The "pmevcntr16_el0" register. */
    DR_REG_PMEVCNTR17_EL0,  /**< The "pmevcntr17_el0" register. */
    DR_REG_PMEVCNTR18_EL0,  /**< The "pmevcntr18_el0" register. */
    DR_REG_PMEVCNTR19_EL0,  /**< The "pmevcntr19_el0" register. */
    DR_REG_PMEVCNTR20_EL0,  /**< The "pmevcntr20_el0" register. */
    DR_REG_PMEVCNTR21_EL0,  /**< The "pmevcntr21_el0" register. */
    DR_REG_PMEVCNTR22_EL0,  /**< The "pmevcntr22_el0" register. */
    DR_REG_PMEVCNTR23_EL0,  /**< The "pmevcntr23_el0" register. */
    DR_REG_PMEVCNTR24_EL0,  /**< The "pmevcntr24_el0" register. */
    DR_REG_PMEVCNTR25_EL0,  /**< The "pmevcntr25_el0" register. */
    DR_REG_PMEVCNTR26_EL0,  /**< The "pmevcntr26_el0" register. */
    DR_REG_PMEVCNTR27_EL0,  /**< The "pmevcntr27_el0" register. */
    DR_REG_PMEVCNTR28_EL0,  /**< The "pmevcntr28_el0" register. */
    DR_REG_PMEVCNTR29_EL0,  /**< The "pmevcntr29_el0" register. */
    DR_REG_PMEVCNTR30_EL0,  /**< The "pmevcntr30_el0" register. */
    DR_REG_PMEVTYPER0_EL0,  /**< The "pmevtyper0_el0" register. */
    DR_REG_PMEVTYPER1_EL0,  /**< The "pmevtyper1_el0" register. */
    DR_REG_PMEVTYPER2_EL0,  /**< The "pmevtyper2_el0" register. */
    DR_REG_PMEVTYPER3_EL0,  /**< The "pmevtyper3_el0" register. */
    DR_REG_PMEVTYPER4_EL0,  /**< The "pmevtyper4_el0" register. */
    DR_REG_PMEVTYPER5_EL0,  /**< The "pmevtyper5_el0" register. */
    DR_REG_PMEVTYPER6_EL0,  /**< The "pmevtyper6_el0" register. */
    DR_REG_PMEVTYPER7_EL0,  /**< The "pmevtyper7_el0" register. */
    DR_REG_PMEVTYPER8_EL0,  /**< The "pmevtyper8_el0" register. */
    DR_REG_PMEVTYPER9_EL0,  /**< The "pmevtyper9_el0" register. */
    DR_REG_PMEVTYPER10_EL0, /**< The "pmevtyper10_el0" register. */
    DR_REG_PMEVTYPER11_EL0, /**< The "pmevtyper11_el0" register. */
    DR_REG_PMEVTYPER12_EL0, /**< The "pmevtyper12_el0" register. */
    DR_REG_PMEVTYPER13_EL0, /**< The "pmevtyper13_el0" register. */
    DR_REG_PMEVTYPER14_EL0, /**< The "pmevtyper14_el0" register. */
    DR_REG_PMEVTYPER15_EL0, /**< The "pmevtyper15_el0" register. */
    DR_REG_PMEVTYPER16_EL0, /**< The "pmevtyper16_el0" register. */
    DR_REG_PMEVTYPER17_EL0, /**< The "pmevtyper17_el0" register. */
    DR_REG_PMEVTYPER18_EL0, /**< The "pmevtyper18_el0" register. */
    DR_REG_PMEVTYPER19_EL0, /**< The "pmevtyper19_el0" register. */
    DR_REG_PMEVTYPER20_EL0, /**< The "pmevtyper20_el0" register. */
    DR_REG_PMEVTYPER21_EL0, /**< The "pmevtyper21_el0" register. */
    DR_REG_PMEVTYPER22_EL0, /**< The "pmevtyper22_el0" register. */
    DR_REG_PMEVTYPER23_EL0, /**< The "pmevtyper23_el0" register. */
    DR_REG_PMEVTYPER24_EL0, /**< The "pmevtyper24_el0" register. */
    DR_REG_PMEVTYPER25_EL0, /**< The "pmevtyper25_el0" register. */
    DR_REG_PMEVTYPER26_EL0, /**< The "pmevtyper26_el0" register. */
    DR_REG_PMEVTYPER27_EL0, /**< The "pmevtyper27_el0" register. */
    DR_REG_PMEVTYPER28_EL0, /**< The "pmevtyper28_el0" register. */
    DR_REG_PMEVTYPER29_EL0, /**< The "pmevtyper29_el0" register. */
    DR_REG_PMEVTYPER30_EL0, /**< The "pmevtyper30_el0" register. */
    DR_REG_PMCCFILTR_EL0,   /**< The "pmccfiltr_el0" register. */
    DR_REG_SPSR_IRQ,        /**< The "spsr_irq" register. */
    DR_REG_SPSR_ABT,        /**< The "spsr_abt" register. */
    DR_REG_SPSR_UND,        /**< The "spsr_und" register. */
    DR_REG_SPSR_FIQ,        /**< The "spsr_fiq" register. */
#    else
    DR_REG_CPSR,                              /**< The "cpsr" register. */
    DR_REG_SPSR,                              /**< The "spsr" register. */
    DR_REG_FPSCR,                             /**< The "fpscr" register. */
#    endif

    /* AArch32 Thread Registers */
    DR_REG_TPIDRURW, /**< User Read/Write Thread ID Register */
    DR_REG_TPIDRURO, /**< User Read-Only Thread ID Register */

#    ifdef AARCH64
    /* SVE vector registers */
    DR_REG_Z0,  /**< The "z0" register. */
    DR_REG_Z1,  /**< The "z1" register. */
    DR_REG_Z2,  /**< The "z2" register. */
    DR_REG_Z3,  /**< The "z3" register. */
    DR_REG_Z4,  /**< The "z4" register. */
    DR_REG_Z5,  /**< The "z5" register. */
    DR_REG_Z6,  /**< The "z6" register. */
    DR_REG_Z7,  /**< The "z7" register. */
    DR_REG_Z8,  /**< The "z8" register. */
    DR_REG_Z9,  /**< The "z9" register. */
    DR_REG_Z10, /**< The "z10" register. */
    DR_REG_Z11, /**< The "z11" register. */
    DR_REG_Z12, /**< The "z12" register. */
    DR_REG_Z13, /**< The "z13" register. */
    DR_REG_Z14, /**< The "z14" register. */
    DR_REG_Z15, /**< The "z15" register. */
    DR_REG_Z16, /**< The "z16" register. */
    DR_REG_Z17, /**< The "z17" register. */
    DR_REG_Z18, /**< The "z18" register. */
    DR_REG_Z19, /**< The "z19" register. */
    DR_REG_Z20, /**< The "z20" register. */
    DR_REG_Z21, /**< The "z21" register. */
    DR_REG_Z22, /**< The "z22" register. */
    DR_REG_Z23, /**< The "z23" register. */
    DR_REG_Z24, /**< The "z24" register. */
    DR_REG_Z25, /**< The "z25" register. */
    DR_REG_Z26, /**< The "z26" register. */
    DR_REG_Z27, /**< The "z27" register. */
    DR_REG_Z28, /**< The "z28" register. */
    DR_REG_Z29, /**< The "z29" register. */
    DR_REG_Z30, /**< The "z30" register. */
    DR_REG_Z31, /**< The "z31" register. */

    /* SVE predicate registers */
    DR_REG_P0,  /**< The "p0" register. */
    DR_REG_P1,  /**< The "p1" register. */
    DR_REG_P2,  /**< The "p2" register. */
    DR_REG_P3,  /**< The "p3" register. */
    DR_REG_P4,  /**< The "p4" register. */
    DR_REG_P5,  /**< The "p5" register. */
    DR_REG_P6,  /**< The "p6" register. */
    DR_REG_P7,  /**< The "p7" register. */
    DR_REG_P8,  /**< The "p8" register. */
    DR_REG_P9,  /**< The "p9" register. */
    DR_REG_P10, /**< The "p10" register. */
    DR_REG_P11, /**< The "p11" register. */
    DR_REG_P12, /**< The "p12" register. */
    DR_REG_P13, /**< The "p13" register. */
    DR_REG_P14, /**< The "p14" register. */
    DR_REG_P15, /**< The "p15" register. */
#    endif

#    ifdef AARCH64
    /* AArch64 Counter/Timer Register(s) */
    DR_REG_CNTVCT_EL0, /**< Virtual Timer Count Register, EL0. */
#    endif

/* Aliases below here: */
#    ifdef AARCH64
    DR_REG_R0 = DR_REG_X0,   /**< Alias for the x0 register. */
    DR_REG_R1 = DR_REG_X1,   /**< Alias for the x1 register. */
    DR_REG_R2 = DR_REG_X2,   /**< Alias for the x2 register. */
    DR_REG_R3 = DR_REG_X3,   /**< Alias for the x3 register. */
    DR_REG_R4 = DR_REG_X4,   /**< Alias for the x4 register. */
    DR_REG_R5 = DR_REG_X5,   /**< Alias for the x5 register. */
    DR_REG_R6 = DR_REG_X6,   /**< Alias for the x6 register. */
    DR_REG_R7 = DR_REG_X7,   /**< Alias for the x7 register. */
    DR_REG_R8 = DR_REG_X8,   /**< Alias for the x8 register. */
    DR_REG_R9 = DR_REG_X9,   /**< Alias for the x9 register. */
    DR_REG_R10 = DR_REG_X10, /**< Alias for the x10 register. */
    DR_REG_R11 = DR_REG_X11, /**< Alias for the x11 register. */
    DR_REG_R12 = DR_REG_X12, /**< Alias for the x12 register. */
    DR_REG_R13 = DR_REG_X13, /**< Alias for the x13 register. */
    DR_REG_R14 = DR_REG_X14, /**< Alias for the x14 register. */
    DR_REG_R15 = DR_REG_X15, /**< Alias for the x15 register. */
    DR_REG_R16 = DR_REG_X16, /**< Alias for the x16 register. */
    DR_REG_R17 = DR_REG_X17, /**< Alias for the x17 register. */
    DR_REG_R18 = DR_REG_X18, /**< Alias for the x18 register. */
    DR_REG_R19 = DR_REG_X19, /**< Alias for the x19 register. */
    DR_REG_R20 = DR_REG_X20, /**< Alias for the x20 register. */
    DR_REG_R21 = DR_REG_X21, /**< Alias for the x21 register. */
    DR_REG_R22 = DR_REG_X22, /**< Alias for the x22 register. */
    DR_REG_R23 = DR_REG_X23, /**< Alias for the x23 register. */
    DR_REG_R24 = DR_REG_X24, /**< Alias for the x24 register. */
    DR_REG_R25 = DR_REG_X25, /**< Alias for the x25 register. */
    DR_REG_R26 = DR_REG_X26, /**< Alias for the x26 register. */
    DR_REG_R27 = DR_REG_X27, /**< Alias for the x27 register. */
    DR_REG_R28 = DR_REG_X28, /**< Alias for the x28 register. */
    DR_REG_R29 = DR_REG_X29, /**< Alias for the x29 register. */
    DR_REG_R30 = DR_REG_X30, /**< Alias for the x30 register. */
    DR_REG_SP = DR_REG_XSP,  /**< The stack pointer register. */
    DR_REG_LR = DR_REG_X30,  /**< The link register. */
#    else
    DR_REG_SP = DR_REG_R13,                   /**< The stack pointer register. */
    DR_REG_LR = DR_REG_R14,                   /**< The link register. */
    DR_REG_PC = DR_REG_R15,                   /**< The program counter register. */
#    endif
    DR_REG_SL = DR_REG_R10,  /**< Alias for the r10 register. */
    DR_REG_FP = DR_REG_R11,  /**< Alias for the r11 register. */
    DR_REG_IP = DR_REG_R12,  /**< Alias for the r12 register. */
#    ifndef AARCH64
    /** Alias for cpsr register (thus this is the full cpsr, not just the apsr bits). */
    DR_REG_APSR = DR_REG_CPSR,
#    endif

    /* AArch64 Thread Registers */
    /** Thread Pointer/ID Register, EL0. */
    DR_REG_TPIDR_EL0 = DR_REG_TPIDRURW,
    /** Thread Pointer/ID Register, Read-Only, EL0. */
    DR_REG_TPIDRRO_EL0 = DR_REG_TPIDRURO,
    /* ARMv7 Thread Registers */
    DR_REG_CP15_C13_2 = DR_REG_TPIDRURW,        /**< User Read/Write Thread ID Register */
    DR_REG_CP15_C13_3 = DR_REG_TPIDRURO,        /**< User Read-Only Thread ID Register */

#    ifdef AARCH64
    DR_REG_LAST_VALID_ENUM = DR_REG_CNTVCT_EL0, /**< Last valid register enum */
    DR_REG_LAST_ENUM = DR_REG_CNTVCT_EL0,       /**< Last value of register enums */
#    else
    DR_REG_LAST_VALID_ENUM = DR_REG_TPIDRURO, /**< Last valid register enum */
    DR_REG_LAST_ENUM = DR_REG_TPIDRURO,       /**< Last value of register enums */
#    endif

#    ifdef AARCH64
    DR_REG_START_64 = DR_REG_X0,  /**< Start of 64-bit general register enum values */
    DR_REG_STOP_64 = DR_REG_XSP,  /**< End of 64-bit general register enum values */
    DR_REG_START_32 = DR_REG_W0,  /**< Start of 32-bit general register enum values */
    DR_REG_STOP_32 = DR_REG_WSP,  /**< End of 32-bit general register enum values */
    DR_REG_START_GPR = DR_REG_X0, /**< Start of full-size general-purpose registers */
    DR_REG_STOP_GPR = DR_REG_XSP, /**< End of full-size general-purpose registers */
#    else
    DR_REG_START_32 = DR_REG_R0,  /**< Start of 32-bit general register enum values */
    DR_REG_STOP_32 = DR_REG_R15,  /**< End of 32-bit general register enum values */
    DR_REG_START_GPR = DR_REG_R0, /**< Start of general register registers */
    DR_REG_STOP_GPR = DR_REG_R15, /**< End of general register registers */
#    endif

    DR_NUM_GPR_REGS = DR_REG_STOP_GPR - DR_REG_START_GPR + 1, /**< Count of GPR regs. */
#    ifdef AARCH64
    DR_NUM_SIMD_VECTOR_REGS = DR_REG_Z31 - DR_REG_Z0 + 1,     /**< Count of SIMD regs. */
#    else
    /* XXX: maybe we want more distinct names that provide counts for 64-bit D or 32-bit
     * S registers.
     */
    DR_NUM_SIMD_VECTOR_REGS = DR_REG_Q15 - DR_REG_Q0 + 1, /**< Count of SIMD regs. */
#    endif

#    ifndef AARCH64
    /** Platform-independent way to refer to stack pointer. */
    DR_REG_XSP = DR_REG_SP,
#    endif

#endif /* X86/ARM */
};

#define REG_NULL DR_REG_NULL
#define REG_INVALID DR_REG_INVALID
#ifndef ARM
#    define REG_START_64 DR_REG_START_64
#    define REG_STOP_64 DR_REG_STOP_64
#endif
#define REG_START_32 DR_REG_START_32
#define REG_STOP_32 DR_REG_STOP_32
#define REG_LAST_VALID_ENUM DR_REG_LAST_VALID_ENUM
#define REG_LAST_ENUM DR_REG_LAST_ENUM
#define REG_XSP DR_REG_XSP

/* Maps sub-registers to their containing register. */
/* Order corresponds to DR_REG_ enum. */
const reg_id_t dr_reg_fixer[] = { REG_NULL,
                                  REG_NULL,

#define XREGS                                                                            \
    DR_REG_X0, DR_REG_X1, DR_REG_X2, DR_REG_X3, DR_REG_X4, DR_REG_X5, DR_REG_X6,         \
        DR_REG_X7, DR_REG_X8, DR_REG_X9, DR_REG_X10, DR_REG_X11, DR_REG_X12, DR_REG_X13, \
        DR_REG_X14, DR_REG_X15, DR_REG_X16, DR_REG_X17, DR_REG_X18, DR_REG_X19,          \
        DR_REG_X20, DR_REG_X21, DR_REG_X22, DR_REG_X23, DR_REG_X24, DR_REG_X25,          \
        DR_REG_X26, DR_REG_X27, DR_REG_X28, DR_REG_X29, DR_REG_X30, DR_REG_XSP,          \
        DR_REG_XZR,
                                  XREGS     /* X0-XSP */
                                      XREGS /* W0-WSP */
#undef XREGS

#define QREGS                                                                            \
    DR_REG_Q0, DR_REG_Q1, DR_REG_Q2, DR_REG_Q3, DR_REG_Q4, DR_REG_Q5, DR_REG_Q6,         \
        DR_REG_Q7, DR_REG_Q8, DR_REG_Q9, DR_REG_Q10, DR_REG_Q11, DR_REG_Q12, DR_REG_Q13, \
        DR_REG_Q14, DR_REG_Q15, DR_REG_Q16, DR_REG_Q17, DR_REG_Q18, DR_REG_Q19,          \
        DR_REG_Q20, DR_REG_Q21, DR_REG_Q22, DR_REG_Q23, DR_REG_Q24, DR_REG_Q25,          \
        DR_REG_Q26, DR_REG_Q27, DR_REG_Q28, DR_REG_Q29, DR_REG_Q30, DR_REG_Q31,
                                          QREGS                 /* Q0-Q31*/
                                              QREGS             /* D0-D31 */
                                                  QREGS         /* S0-S31 */
                                                      QREGS     /* H0-H31 */
                                                          QREGS /* B0-B31 */
#undef QREGS

    DR_REG_NZCV, DR_REG_FPCR, DR_REG_FPSR,
    DR_REG_MDCCSR_EL0, DR_REG_DBGDTR_EL0, DR_REG_DBGDTRRX_EL0, DR_REG_SP_EL0,
    DR_REG_SPSEL, DR_REG_DAIFSET, DR_REG_DAIFCLR, DR_REG_CURRENTEL, DR_REG_PAN,
    DR_REG_UAO, DR_REG_CTR_EL0, DR_REG_DCZID_EL0, DR_REG_RNDR, DR_REG_RNDRRS,
    DR_REG_DAIF, DR_REG_DIT, DR_REG_SSBS, DR_REG_TCO, DR_REG_DSPSR_EL0,
    DR_REG_DLR_EL0, DR_REG_PMCR_EL0, DR_REG_PMCNTENSET_EL0,
    DR_REG_PMCNTENCLR_EL0, DR_REG_PMOVSCLR_EL0,
    DR_REG_PMSWINC_EL0, DR_REG_PMSELR_EL0, DR_REG_PMCEID0_EL0,
    DR_REG_PMCEID1_EL0, DR_REG_PMCCNTR_EL0, DR_REG_PMXEVTYPER_EL0,
    DR_REG_PMXEVCNTR_EL0, DR_REG_PMUSERENR_EL0, DR_REG_PMOVSSET_EL0,
    DR_REG_SCXTNUM_EL0, DR_REG_CNTFRQ_EL0, DR_REG_CNTPCT_EL0,
    DR_REG_CNTP_TVAL_EL0, DR_REG_CNTP_CTL_EL0, DR_REG_CNTP_CVAL_EL0,
    DR_REG_CNTV_TVAL_EL0, DR_REG_CNTV_CTL_EL0, DR_REG_CNTV_CVAL_EL0,
    DR_REG_PMEVCNTR0_EL0, DR_REG_PMEVCNTR1_EL0, DR_REG_PMEVCNTR2_EL0,
    DR_REG_PMEVCNTR3_EL0, DR_REG_PMEVCNTR4_EL0, DR_REG_PMEVCNTR5_EL0,
    DR_REG_PMEVCNTR6_EL0, DR_REG_PMEVCNTR7_EL0, DR_REG_PMEVCNTR8_EL0,
    DR_REG_PMEVCNTR9_EL0, DR_REG_PMEVCNTR10_EL0, DR_REG_PMEVCNTR11_EL0,
    DR_REG_PMEVCNTR12_EL0, DR_REG_PMEVCNTR13_EL0, DR_REG_PMEVCNTR14_EL0,
    DR_REG_PMEVCNTR15_EL0, DR_REG_PMEVCNTR16_EL0, DR_REG_PMEVCNTR17_EL0,
    DR_REG_PMEVCNTR18_EL0, DR_REG_PMEVCNTR19_EL0, DR_REG_PMEVCNTR20_EL0,
    DR_REG_PMEVCNTR21_EL0, DR_REG_PMEVCNTR22_EL0, DR_REG_PMEVCNTR23_EL0,
    DR_REG_PMEVCNTR24_EL0, DR_REG_PMEVCNTR25_EL0, DR_REG_PMEVCNTR26_EL0,
    DR_REG_PMEVCNTR27_EL0, DR_REG_PMEVCNTR28_EL0, DR_REG_PMEVCNTR29_EL0,
    DR_REG_PMEVCNTR30_EL0, DR_REG_PMEVTYPER0_EL0, DR_REG_PMEVTYPER1_EL0,
    DR_REG_PMEVTYPER2_EL0, DR_REG_PMEVTYPER3_EL0, DR_REG_PMEVTYPER4_EL0,
    DR_REG_PMEVTYPER5_EL0, DR_REG_PMEVTYPER6_EL0, DR_REG_PMEVTYPER7_EL0,
    DR_REG_PMEVTYPER8_EL0, DR_REG_PMEVTYPER9_EL0, DR_REG_PMEVTYPER10_EL0,
    DR_REG_PMEVTYPER11_EL0, DR_REG_PMEVTYPER12_EL0, DR_REG_PMEVTYPER13_EL0,
    DR_REG_PMEVTYPER14_EL0, DR_REG_PMEVTYPER15_EL0, DR_REG_PMEVTYPER16_EL0,
    DR_REG_PMEVTYPER17_EL0, DR_REG_PMEVTYPER18_EL0, DR_REG_PMEVTYPER19_EL0,
    DR_REG_PMEVTYPER20_EL0, DR_REG_PMEVTYPER21_EL0, DR_REG_PMEVTYPER22_EL0,
    DR_REG_PMEVTYPER23_EL0, DR_REG_PMEVTYPER24_EL0, DR_REG_PMEVTYPER25_EL0,
    DR_REG_PMEVTYPER26_EL0, DR_REG_PMEVTYPER27_EL0, DR_REG_PMEVTYPER28_EL0,
    DR_REG_PMEVTYPER29_EL0, DR_REG_PMEVTYPER30_EL0, DR_REG_PMCCFILTR_EL0,
    DR_REG_SPSR_IRQ, DR_REG_SPSR_ABT, DR_REG_SPSR_UND, DR_REG_SPSR_FIQ,
    DR_REG_TPIDR_EL0, DR_REG_TPIDRRO_EL0
};

/* Memory operand sizes (with Intel's corresponding size names noted).
 *
 * Intel's size names are listed in 'Appendix A Opcode Map (Intel SDM Volume 2)'
 * specifically A.2.2 Codes for Operand Type
 *
 * For register operands, the DR_REG_ constants are used, which implicitly
 * state a size (e.g., DR_REG_CX is 2 bytes).
 * Use the type opnd_size_t for these values (we avoid typedef-ing the
 * enum, as its storage size is compiler-specific).  opnd_size_t is a
 * byte, so the largest value here needs to be <= 255.
 */
enum {
    /* For x86, register enum values are used for TYPE_*REG but we only use them
     * as opnd_size_t when we have the type available, so we can overlap
     * the two enums. If needed, the function template_optype_is_reg can be used
     * to check whether the operand type has an implicit size and stores the reg enum
     * instead of the size enum.
     * The reg_id_t type is now wider, but for x86 we ensure our values
     * all fit via an assert in d_r_arch_init().
     */
    OPSZ_NA = 0, /**< Sentinel value: not a valid size. */
    OPSZ_FIRST = OPSZ_NA,
    OPSZ_0,   /**< 0 bytes, for "sizeless" operands (for Intel, code
               * 'm': used for both start addresses (lea, invlpg) and
               * implicit constants (rol, fldl2e, etc.) */
    OPSZ_1,   /**< 1 byte (for Intel, code 'b') */
    OPSZ_2,   /**< 2 bytes (for Intel, code 'w') */
    OPSZ_4,   /**< 4 bytes (for Intel, code 'd','si') */
    OPSZ_6,   /**< 6 bytes (for Intel, code 'p','s') */
    OPSZ_8,   /**< 8 bytes (for Intel, code 'q','pi') */
    OPSZ_10,  /**< Intel 's' 64-bit, or double extended precision floating point
               * (latter used by fld, fstp, fbld, fbstp) */
    OPSZ_16,  /**< 16 bytes (for Intel, code 'dq','ps','pd','ss','sd', or AMD 'o') */
    OPSZ_14,  /**< FPU operating environment with short data size (fldenv, fnstenv) */
    OPSZ_28,  /**< FPU operating environment with normal data size (fldenv, fnstenv) */
    OPSZ_94,  /**< FPU state with short data size (fnsave, frstor) */
    OPSZ_108, /**< FPU state with normal data size (fnsave, frstor) */
    OPSZ_512, /**< FPU, MMX, XMM state (fxsave, fxrstor) */
    /**
     * The following sizes (OPSZ_*_short*) vary according to the cs segment and the
     * operand size prefix.  This IR assumes that the cs segment is set to the
     * default operand size.  The operand size prefix then functions to shrink the
     * size.  The IR does not explicitly mark the prefix; rather, a shortened size is
     * requested in the operands themselves, with the IR adding the prefix at encode
     * time.  Normally the fixed sizes above should be used rather than these
     * variable sizes, which are used internally by the IR and should only be
     * externally specified when building an operand in order to be flexible and
     * allow other operands to decide the size for the instruction (the prefix
     * applies to the entire instruction).
     */
    OPSZ_2_short1,        /**< Intel 'c': 2/1 bytes ("2/1" means 2 bytes normally, but if
                           * another operand requests a short size then this size can
                           * accommodate by shifting to its short size, which is 1
                           * byte). */
    OPSZ_4_short2,        /**< Intel 'z': 4/2 bytes */
    OPSZ_4_rex8_short2,   /**< Intel 'v': 8/4/2 bytes */
    OPSZ_4_rex8,          /**< Intel 'd/q' (like 'v' but never 2 bytes) or 'y'. */
    OPSZ_6_irex10_short4, /**< Intel 'p': On Intel processors this is 10/6/4 bytes for
                           * segment selector + address.  On AMD processors this is
                           * 6/4 bytes for segment selector + address (rex is ignored). */
    OPSZ_8_short2,        /**< partially resolved 4x8_short2 */
    OPSZ_8_short4,        /**< Intel 'a': pair of 4_short2 (bound) */
    OPSZ_28_short14,      /**< FPU operating env variable data size (fldenv, fnstenv) */
    OPSZ_108_short94,     /**< FPU state with variable data size (fnsave, frstor) */
    /** Varies by 32-bit versus 64-bit processor mode. */
    OPSZ_4x8,  /**< Full register size with no variation by prefix.
                *   Used for control and debug register moves and for Intel MPX. */
    OPSZ_6x10, /**< Intel 's': 6-byte (10-byte for 64-bit mode) table base + limit */
    /**
     * Stack operands not only vary by operand size specifications but also
     * by 32-bit versus 64-bit processor mode.
     */
    OPSZ_4x8_short2,    /**< Intel 'v'/'d64' for stack operations.
                         * Also 64-bit address-size specified operands, which are
                         * short4 rather than short2 in 64-bit mode (but short2 in
                         * 32-bit mode).
                         * Note that this IR does not distinguish extra stack
                         * operations performed by OP_enter w/ non-zero immed.
                         */
    OPSZ_4x8_short2xi8, /**< Intel 'f64': 4_short2 for 32-bit, 8_short2 for 64-bit AMD,
                         *   always 8 for 64-bit Intel */
    OPSZ_4_short2xi4,   /**< Intel 'f64': 4_short2 for 32-bit or 64-bit AMD,
                         *   always 4 for 64-bit Intel */
    /**
     * The following 3 sizes differ based on whether the modrm chooses a
     * register or memory.
     */
    OPSZ_1_reg4,  /**< Intel Rd/Mb: zero-extends if reg; used by pextrb */
    OPSZ_2_reg4,  /**< Intel Rd/Mw: zero-extends if reg; used by pextrw */
    OPSZ_4_reg16, /**< Intel Udq/Md: 4 bytes of xmm or 4 bytes of memory;
                   *   used by insertps. */
    /* Sizes used by new instructions */
    OPSZ_xsave,           /**< Size is > 512 bytes: use cpuid to determine.
                           * Used for FPU, MMX, XMM, etc. state by xsave and xrstor. */
    OPSZ_12,              /**< 12 bytes: 32-bit iret */
    OPSZ_32,              /**< 32 bytes: pusha/popa
                           * Also Intel 'qq','pd','ps','x': 32 bytes (256 bits) */
    OPSZ_40,              /**< 40 bytes: 64-bit iret */
    OPSZ_32_short16,      /**< unresolved pusha/popa */
    OPSZ_8_rex16,         /**< cmpxcgh8b/cmpxchg16b */
    OPSZ_8_rex16_short4,  /**< Intel 'v' * 2 (far call/ret) */
    OPSZ_12_rex40_short6, /**< unresolved iret */
    OPSZ_16_vex32,        /**< 16 or 32 bytes depending on VEX.L (AMD/Intel 'x'). */
    OPSZ_15, /**< All but one byte of an xmm register (used by OP_vpinsrb). */

    /* Needed for ARM.  We share the same namespace for now */
    OPSZ_3, /**< 3 bytes */
    /* gpl_list_num_bits assumes OPSZ_ includes every value from 1b to 12b
     * (except 8b/OPSZ_1) in order
     */
    OPSZ_1b,  /**< 1 bit */
    OPSZ_2b,  /**< 2 bits */
    OPSZ_3b,  /**< 3 bits */
    OPSZ_4b,  /**< 4 bits */
    OPSZ_5b,  /**< 5 bits */
    OPSZ_6b,  /**< 6 bits */
    OPSZ_7b,  /**< 7 bits */
    OPSZ_9b,  /**< 9 bits */
    OPSZ_10b, /**< 10 bits */
    OPSZ_11b, /**< 11 bits */
    OPSZ_12b, /**< 12 bits */
    OPSZ_20b, /**< 20 bits */
    OPSZ_25b, /**< 25 bits */
    /**
     * At encode or decode time, the size will match the size of the
     * register list operand in the containing instruction's operands.
     */
    OPSZ_VAR_REGLIST,
    OPSZ_20,  /**< 20 bytes.  Needed for load/store of register lists. */
    OPSZ_24,  /**< 24 bytes.  Needed for load/store of register lists. */
    OPSZ_36,  /**< 36 bytes.  Needed for load/store of register lists. */
    OPSZ_44,  /**< 44 bytes.  Needed for load/store of register lists. */
    OPSZ_48,  /**< 48 bytes.  Needed for load/store of register lists. */
    OPSZ_52,  /**< 52 bytes.  Needed for load/store of register lists. */
    OPSZ_56,  /**< 56 bytes.  Needed for load/store of register lists. */
    OPSZ_60,  /**< 60 bytes.  Needed for load/store of register lists. */
    OPSZ_64,  /**< 64 bytes.  Needed for load/store of register lists.
               * Also Intel: 64 bytes (512 bits) */
    OPSZ_68,  /**< 68 bytes.  Needed for load/store of register lists. */
    OPSZ_72,  /**< 72 bytes.  Needed for load/store of register lists. */
    OPSZ_76,  /**< 76 bytes.  Needed for load/store of register lists. */
    OPSZ_80,  /**< 80 bytes.  Needed for load/store of register lists. */
    OPSZ_84,  /**< 84 bytes.  Needed for load/store of register lists. */
    OPSZ_88,  /**< 88 bytes.  Needed for load/store of register lists. */
    OPSZ_92,  /**< 92 bytes.  Needed for load/store of register lists. */
    OPSZ_96,  /**< 96 bytes.  Needed for load/store of register lists. */
    OPSZ_100, /**< 100 bytes. Needed for load/store of register lists. */
    OPSZ_104, /**< 104 bytes. Needed for load/store of register lists. */
    /* OPSZ_108 already exists */
    OPSZ_112,             /**< 112 bytes. Needed for load/store of register lists. */
    OPSZ_116,             /**< 116 bytes. Needed for load/store of register lists. */
    OPSZ_120,             /**< 120 bytes. Needed for load/store of register lists. */
    OPSZ_124,             /**< 124 bytes. Needed for load/store of register lists. */
    OPSZ_128,             /**< 128 bytes. Needed for load/store of register lists. */
    OPSZ_SCALABLE,        /**< Scalable size for SVE vector registers. */
    OPSZ_SCALABLE_PRED,   /**< Scalable size for SVE predicate registers. */
    OPSZ_16_vex32_evex64, /**< 16, 32, or 64 bytes depending on EVEX.L and EVEX.LL'. */
    OPSZ_vex32_evex64,    /**< 32 or 64 bytes depending on EVEX.L and EVEX.LL'. */
    OPSZ_16_of_32_evex64, /**< 128 bits: half of YMM or quarter of ZMM depending on
                           * EVEX.LL'.
                           */
    OPSZ_32_of_64,        /**< 256 bits: half of ZMM. */
    OPSZ_4_of_32_evex64,  /**< 32 bits: can be part of YMM or ZMM register. */
    OPSZ_8_of_32_evex64,  /**< 64 bits: can be part of YMM or ZMM register. */
    OPSZ_8x16, /**< 8 or 16 bytes, but not based on rex prefix, instead dependent
                * on 32-bit/64-bit mode.
                */
    /* Add new size here.  Also update size_names[] in decode_shared.c along with
     * the size routines in opnd_shared.c.
     */
    OPSZ_LAST,
};

#    define OPSZ_PTR OPSZ_8      /**< Operand size for pointer values. */
#    define OPSZ_STACK OPSZ_8    /**< Operand size for stack push/pop operand sizes. */
#    define OPSZ_PTR_DBL OPSZ_16 /**< Double-pointer-sized. */
#    define OPSZ_PTR_HALF OPSZ_4 /**< Half-pointer-sized. */
#ifdef AARCH64
#    define OPSZ_sys OPSZ_1 /**< Operand size for sys instruction memory reference. */
#endif

typedef byte opnd_size_t; /**< The type of an OPSZ_ enum value. */

/* returns the OPSZ_ field appropriate for the register */
opnd_size_t
reg_get_size(reg_id_t reg)
{
#ifdef X64
    if (reg >= REG_START_64 && reg <= REG_STOP_64)
        return OPSZ_8;
#endif
    if (reg >= REG_START_32 && reg <= REG_STOP_32)
        return OPSZ_4;
#ifdef X86
    if (reg >= REG_START_8 && reg <= REG_STOP_8)
        return OPSZ_1;
#endif
#if defined(X86) && defined(X64)
    if (reg >= REG_START_x64_8 && reg <= REG_STOP_x64_8) /* alternates to AH-BH */
        return OPSZ_1;
#endif
#ifdef X86
    if (reg >= REG_START_16 && reg <= REG_STOP_16)
        return OPSZ_2;
    if (reg >= REG_START_MMX && reg <= REG_STOP_MMX)
        return OPSZ_8;
    if (reg >= DR_REG_START_XMM && reg <= DR_REG_STOP_XMM)
        return OPSZ_16;
    if (reg >= DR_REG_START_YMM && reg <= DR_REG_STOP_YMM)
        return OPSZ_32;
    if (reg >= DR_REG_START_ZMM && reg <= DR_REG_STOP_ZMM)
        return OPSZ_64;
    if (reg >= DR_REG_START_OPMASK && reg <= DR_REG_STOP_OPMASK) {
        /* The default is 16 bits wide. The register may be up to 64 bits wide with
         * the AVX-512BW extension, which depends on the processor. The number of
         * bits actually used depends on the vector type of the instruction.
         */
        return OPSZ_8;
    }
    if (reg >= DR_REG_START_BND && reg <= DR_REG_STOP_BND)
        return IF_X64_ELSE(OPSZ_16, OPSZ_8);
    if (reg >= REG_START_SEGMENT && reg <= REG_STOP_SEGMENT)
        return OPSZ_2;
    if (reg >= REG_START_DR && reg <= REG_STOP_DR)
        return IF_X64_ELSE(OPSZ_8, OPSZ_4);
    if (reg >= REG_START_CR && reg <= REG_STOP_CR)
        return IF_X64_ELSE(OPSZ_8, OPSZ_4);
    /* i#176 add reg size handling for floating point registers */
    if (reg >= REG_START_FLOAT && reg <= REG_STOP_FLOAT)
        return OPSZ_10;
#elif defined(AARCHXX)
    if (reg >= DR_REG_Q0 && reg <= DR_REG_Q31)
        return OPSZ_16;
    if (reg >= DR_REG_D0 && reg <= DR_REG_D31)
        return OPSZ_8;
    if (reg >= DR_REG_S0 && reg <= DR_REG_S31)
        return OPSZ_4;
    if (reg >= DR_REG_H0 && reg <= DR_REG_H31)
        return OPSZ_2;
    if (reg >= DR_REG_B0 && reg <= DR_REG_B31)
        return OPSZ_1;
#    ifdef ARM
    if (reg >= DR_REG_CR0 && reg <= DR_REG_CR15)
        return OPSZ_PTR;
    if (reg >= DR_REG_CPSR && reg <= DR_REG_FPSCR)
        return OPSZ_4;
#    elif defined(AARCH64)
    if (reg == DR_REG_XZR)
        return OPSZ_8;
    if (reg == DR_REG_WZR)
        return OPSZ_4;
    if (reg >= DR_REG_MDCCSR_EL0 && reg <= DR_REG_SPSR_FIQ)
        return OPSZ_8;
    if (reg >= DR_REG_Z0 && reg <= DR_REG_Z31)
        return OPSZ_SCALABLE;
    if (reg >= DR_REG_P0 && reg <= DR_REG_P15)
        return OPSZ_SCALABLE_PRED;
    if (reg == DR_REG_CNTVCT_EL0)
        return OPSZ_8;
    if (reg >= DR_REG_NZCV && reg <= DR_REG_FPSR)
        return OPSZ_8;
#    endif
    if (reg == DR_REG_TPIDRURW || reg == DR_REG_TPIDRURO)
        return OPSZ_PTR;
#endif
    CLIENT_ASSERT(false, "reg_get_size: invalid register");
    return OPSZ_NA;
}

#    define OPND_GET_REG(opnd)                                                          \
        (CLIENT_ASSERT_(opnd_is_reg(opnd), "opnd_get_reg called on non-reg opnd")(opnd) \
             .value.reg)

reg_id_t
opnd_get_reg(opnd_t opnd)
{
    return OPND_GET_REG(opnd);
}


opnd_size_t
opnd_get_size(opnd_t opnd)
{
    switch (opnd.kind) {
    case REG_kind: return (opnd.size == 0 ? reg_get_size(opnd_get_reg(opnd)) : opnd.size);
    case IMMED_INTEGER_kind:
    case IMMED_FLOAT_kind:
    case IMMED_DOUBLE_kind:
    case BASE_DISP_kind:
#if defined(X64) || defined(ARM)
    case REL_ADDR_kind:
#endif
#ifdef X64
    case ABS_ADDR_kind:
#endif
    case MEM_INSTR_kind:
    case INSTR_kind: return opnd.size;
    case PC_kind: return OPSZ_PTR;
    case FAR_PC_kind:
    case FAR_INSTR_kind: return OPSZ_6_irex10_short4;
    case NULL_kind: return OPSZ_NA;
    default: CLIENT_ASSERT(false, "opnd_get_size: unknown opnd type"); return OPSZ_NA;
    }
}

/* NOTE: requires caller to be under PRESERVE_FLOATING_POINT_STATE */
opnd_t
opnd_create_immed_float(float i)
{
    opnd_t opnd;
    opnd.kind = IMMED_FLOAT_kind;
    /* Note that manipulating floats and doubles by copying in this way can
     * result in using FP load/store instructions which can trigger any pending
     * FP exception (i#386).
     */
    opnd.value.immed_float = i;
    /* currently only used for implicit constants that have no size */
    opnd.size = OPSZ_0;
    return opnd;
}


/**
 * These flags describe operations performed on the value of a source register
 * before it is combined with other sources as part of the behavior of the
 * containing instruction, or operations performed on an index register or
 * displacement before it is added to or subtracted from the base register.
 */
typedef enum _dr_opnd_flags_t {
    /** This register's value is negated prior to use in the containing instruction. */
    DR_OPND_NEGATED = 0x01,
    /**
     * This register's value is shifted prior to use in the containing instruction.
     * This flag is for informational purposes only and is not guaranteed to
     * be consistent with the shift type of an index register or displacement
     * if the latter are set without using opnd_set_index_shift() or if an
     * instruction is created without using high-level API routines.
     * This flag is also ignored for encoding and will not apply a shift
     * on its own.
     */
    DR_OPND_SHIFTED = 0x02,
    /**
     * This operand should be combined with an adjacent operand to create a
     * single value.  This flag is typically used on immediates: e.g., for ARM's
     * OP_vbic_i64, two 32-bit immediate operands should be interpreted as the
     * low and high parts of a 64-bit value.
     */
    DR_OPND_MULTI_PART = 0x04,
    /**
     * This immediate integer operand should be interpreted as an ARM/AArch64 shift type.
     */
    DR_OPND_IS_SHIFT = 0x08,
    /** A hint indicating that this register operand is part of a register list. */
    DR_OPND_IN_LIST = 0x10,
    /**
     * This register's value is extended prior to use in the containing instruction.
     * This flag is for informational purposes only and is not guaranteed to
     * be consistent with the shift type of an index register or displacement
     * if the latter are set without using opnd_set_index_extend() or if an
     * instruction is created without using high-level API routines.
     * This flag is also ignored for encoding and will not apply a shift
     * on its own.
     */
    DR_OPND_EXTENDED = 0x20,
    /** This immediate integer operand should be interpreted as an AArch64 extend type. */
    DR_OPND_IS_EXTEND = 0x40,
    /** This immediate integer operand should be interpreted as an AArch64 condition. */
    DR_OPND_IS_CONDITION = 0x80,
} dr_opnd_flags_t;

/**
 * These flags describe how the index register in a memory reference is extended
 * before being optionally shifted and added to the base register. They also describe
 * how a general source register is extended before being used in its containing
 * instruction.
 */
typedef enum _dr_extend_type_t {
    DR_EXTEND_UXTB = 0, /**< Unsigned extend byte. */
    DR_EXTEND_UXTH,     /**< Unsigned extend halfword. */
    DR_EXTEND_UXTW,     /**< Unsigned extend word. */
    DR_EXTEND_UXTX,     /**< Unsigned extend doubleword (a no-op). */
    DR_EXTEND_SXTB,     /**< Signed extend byte. */
    DR_EXTEND_SXTH,     /**< Signed extend halfword. */
    DR_EXTEND_SXTW,     /**< Signed extend word. */
    DR_EXTEND_SXTX,     /**< Signed extend doubleword (a no-op). */
} dr_extend_type_t;

static inline void
opnd_set_disp_helper(opnd_t *opnd, int disp)
{
    IF_ARM_ELSE(
        {
            if (disp < 0) {
                opnd->aux.flags |= DR_OPND_NEGATED;
                opnd->value.base_disp.disp = -disp;
            } else
                opnd->value.base_disp.disp = disp;
        },
        { opnd->value.base_disp.disp = disp; });
}

opnd_t
opnd_create_far_base_disp_ex(reg_id_t seg, reg_id_t base_reg, reg_id_t index_reg,
                             int scale, int disp, opnd_size_t size, bool encode_zero_disp,
                             bool force_full_disp, bool disp_short_addr)
{
    opnd_t opnd;
    opnd.kind = BASE_DISP_kind;
    CLIENT_ASSERT(size < OPSZ_LAST_ENUM, "opnd_create_*base_disp*: invalid size");
    opnd.size = size;
    CLIENT_ASSERT(scale == 0 || scale == 1 || scale == 2 || scale == 4 || scale == 8,
                  "opnd_create_*base_disp*: invalid scale");
    IF_X86(CLIENT_ASSERT(index_reg == REG_NULL || scale > 0,
                         "opnd_create_*base_disp*: index requires scale"));
    CLIENT_ASSERT(
        seg == REG_NULL IF_X86(|| (seg >= REG_START_SEGMENT && seg <= REG_STOP_SEGMENT)),
        "opnd_create_*base_disp*: invalid segment");
    CLIENT_ASSERT(base_reg <= REG_LAST_ENUM, "opnd_create_*base_disp*: invalid base");
    CLIENT_ASSERT(index_reg <= REG_LAST_ENUM, "opnd_create_*base_disp*: invalid index");
    CLIENT_ASSERT_BITFIELD_TRUNCATE(SCALE_SPECIFIER_BITS, scale,
                                    "opnd_create_*base_disp*: invalid scale");
    /* reg_id_t is now a ushort, but we can only accept low values */
    CLIENT_ASSERT_BITFIELD_TRUNCATE(REG_SPECIFIER_BITS, base_reg,
                                    "opnd_create_*base_disp*: invalid base");
    CLIENT_ASSERT_BITFIELD_TRUNCATE(REG_SPECIFIER_BITS, index_reg,
                                    "opnd_create_*base_disp*: invalid index");
    IF_X86_ELSE({ opnd.aux.segment = seg; },
                {
                    opnd.aux.flags = 0;
                    CLIENT_ASSERT(
                        disp == 0 || index_reg == REG_NULL,
                        "opnd_create_*base_disp*: cannot have both disp and index");
                });
    opnd_set_disp_helper(&opnd, disp);
    opnd.value.base_disp.base_reg = base_reg;
    opnd.value.base_disp.index_reg = index_reg;
    opnd.value.base_disp.pre_index = true;
    opnd.value.base_disp.extend_type = DR_EXTEND_UXTX;
    opnd.value.base_disp.scaled = false;
    return opnd;
}

opnd_t
opnd_create_base_disp_ex(reg_id_t base_reg, reg_id_t index_reg, int scale, int disp,
                         opnd_size_t size, bool encode_zero_disp, bool force_full_disp,
                         bool disp_short_addr)
{
    return opnd_create_far_base_disp_ex(REG_NULL, base_reg, index_reg, scale, disp, size,
                                        encode_zero_disp, force_full_disp,
                                        disp_short_addr);
}

opnd_t
opnd_create_base_disp(reg_id_t base_reg, reg_id_t index_reg, int scale, int disp,
                      opnd_size_t size)
{
    return opnd_create_far_base_disp_ex(REG_NULL, base_reg, index_reg, scale, disp, size,
                                        false, false, false);
}

DR_API
opnd_size_t
opnd_size_from_bytes(uint bytes)
{
    switch (bytes) {
    case 0: return OPSZ_0;
    case 1: return OPSZ_1;
    case 2: return OPSZ_2;
    case 3: return OPSZ_3;
    case 4: return OPSZ_4;
    case 6: return OPSZ_6;
    case 8: return OPSZ_8;
    case 10: return OPSZ_10;
    case 12: return OPSZ_12;
    case 14: return OPSZ_14;
    case 15: return OPSZ_15;
    case 16: return OPSZ_16;
    case 20: return OPSZ_20;
    case 24: return OPSZ_24;
    case 28: return OPSZ_28;
    case 32: return OPSZ_32;
    case 36: return OPSZ_36;
    case 40: return OPSZ_40;
    case 44: return OPSZ_44;
    case 48: return OPSZ_48;
    case 52: return OPSZ_52;
    case 56: return OPSZ_56;
    case 60: return OPSZ_60;
    case 64: return OPSZ_64;
    case 68: return OPSZ_68;
    case 72: return OPSZ_72;
    case 76: return OPSZ_76;
    case 80: return OPSZ_80;
    case 84: return OPSZ_84;
    case 88: return OPSZ_88;
    case 92: return OPSZ_92;
    case 94: return OPSZ_94;
    case 96: return OPSZ_96;
    case 100: return OPSZ_100;
    case 104: return OPSZ_104;
    case 108: return OPSZ_108;
    case 112: return OPSZ_112;
    case 116: return OPSZ_116;
    case 120: return OPSZ_120;
    case 124: return OPSZ_124;
    case 128: return OPSZ_128;
    case 512: return OPSZ_512;
    default: return OPSZ_NA;
    }
}


/* PR 253327: We represent rip-relative w/ an address-size prefix
 * (i.e., 32 bits instead of 64) as simply having the top 32 bits of
 * "addr" zeroed out.  This means that we never encode an address
 * prefix, and we if one already exists in the raw bits we have to go
 * looking for it at encode time.
 */
opnd_t
opnd_create_far_rel_addr(reg_id_t seg, void *addr, opnd_size_t data_size)
{
    opnd_t opnd;
    opnd.kind = REL_ADDR_kind;
    CLIENT_ASSERT(data_size < OPSZ_LAST_ENUM, "opnd_create_base_disp: invalid size");
    opnd.size = data_size;
    CLIENT_ASSERT(
        seg == REG_NULL IF_X86(|| (seg >= REG_START_SEGMENT && seg <= REG_STOP_SEGMENT)),
        "opnd_create_far_rel_addr: invalid segment");
    IF_X86(opnd.aux.segment = seg);
    opnd.value.addr = addr;
    return opnd;
}

opnd_t
opnd_create_rel_addr(void *addr, opnd_size_t data_size)
{
    return opnd_create_far_rel_addr(REG_NULL, addr, data_size);
}

#    define OPND_IS_BASE_DISP(op) ((op).kind == BASE_DISP_kind)

bool
opnd_is_base_disp(opnd_t op)
{
    return OPND_IS_BASE_DISP(op);
}

#    define GET_BASE_DISP(opnd)                                                 \
        (CLIENT_ASSERT_(opnd_is_base_disp(opnd),                                \
                        "opnd_get_base_disp called on invalid opnd type")(opnd) \
             .value.base_disp)
#    define OPND_GET_BASE(opnd) (GET_BASE_DISP(opnd).base_reg)

reg_id_t
opnd_get_base(opnd_t opnd)
{
    return OPND_GET_BASE(opnd);
}

#define OPND_GET_INDEX(opnd) (GET_BASE_DISP(opnd).index_reg)
	
reg_id_t
opnd_get_index(opnd_t opnd)
{
    return OPND_GET_INDEX(opnd);
}

#    define OPND_GET_DISP(opnd) (GET_BASE_DISP(opnd).disp)

int
opnd_get_disp(opnd_t opnd)
{
    return OPND_GET_DISP(opnd);
}

#        define OPND_GET_SEGMENT(opnd) DR_REG_NULL

reg_id_t
opnd_get_segment(opnd_t opnd)
{
    return OPND_GET_SEGMENT(opnd);
}

#                define OPND_IS_REL_ADDR(op) ((op).kind == REL_ADDR_kind)

bool
opnd_is_rel_addr(opnd_t op)
{
    return OPND_IS_REL_ADDR(op);
}

/* We allow overlap between ABS_ADDR_kind and BASE_DISP_kind w/ no base or index */
bool
opnd_is_abs_base_disp(opnd_t opnd)
{
    return (opnd_is_base_disp(opnd) && opnd_get_base(opnd) == REG_NULL &&
            opnd_get_index(opnd) == REG_NULL);
}
bool
opnd_is_abs_addr(opnd_t opnd)
{
    return IF_X64(opnd.kind == ABS_ADDR_kind ||) opnd_is_abs_base_disp(opnd);
}
bool
opnd_is_near_abs_addr(opnd_t opnd)
{
    return opnd_is_abs_addr(opnd) IF_X86(&&opnd.aux.segment == REG_NULL);
}
bool
opnd_is_far_abs_addr(opnd_t opnd)
{
    return IF_X86_ELSE(opnd_is_abs_addr(opnd) && opnd.aux.segment != REG_NULL, false);
}

void *
opnd_get_addr(opnd_t opnd)
{
    /* check base-disp first since opnd_is_abs_addr() says yes for it */
    if (opnd_is_abs_base_disp(opnd))
        return (void *)(ptr_int_t)opnd_get_disp(opnd);
#if defined(X64) || defined(ARM)
    if (IF_X64(opnd_is_abs_addr(opnd) ||) opnd_is_rel_addr(opnd))
        return opnd.value.addr;
#endif
    CLIENT_ASSERT(false, "opnd_get_addr called on invalid opnd type");
    return NULL;
}

void
opnd_set_flags(opnd_t *opnd, dr_opnd_flags_t flags)
{
    CLIENT_ASSERT(opnd_is_reg(*opnd) || opnd_is_base_disp(*opnd) ||
                      opnd_is_immed_int(*opnd),
                  "opnd_set_flags called on non-reg non-base-disp non-immed-int opnd");
    opnd->aux.flags = flags;
}

opnd_t
opnd_add_flags(opnd_t opnd, dr_opnd_flags_t flags)
{
    opnd_set_flags(&opnd, flags | opnd.aux.flags);
    return opnd;
}

bool
opnd_set_index_extend(opnd_t *opnd, dr_extend_type_t extend, bool scaled)
{
    if (!opnd_is_base_disp(*opnd)) {
        CLIENT_ASSERT(false, "opnd_set_index_shift called on invalid opnd type");
        return false;
    }
    if (extend > 7) {
        CLIENT_ASSERT(false, "opnd index extend: invalid extend type");
        return false;
    }
    opnd->value.base_disp.extend_type = extend;
    opnd->value.base_disp.scaled = scaled;
    return true;
}

opnd_t
opnd_create_base_disp_aarch64(reg_id_t base_reg, reg_id_t index_reg,
                              dr_extend_type_t extend_type, bool scaled, int disp,
                              dr_opnd_flags_t flags, opnd_size_t size)
{
    opnd_t opnd;
    opnd.kind = BASE_DISP_kind;
    CLIENT_ASSERT(size < OPSZ_LAST_ENUM, "opnd_create_*base_disp*: invalid size");
    opnd.size = size;
    CLIENT_ASSERT(disp == 0 || index_reg == REG_NULL,
                  "opnd_create_base_disp_aarch64: cannot have both disp and index");
    CLIENT_ASSERT(base_reg <= REG_LAST_ENUM,
                  "opnd_create_base_disp_aarch64: invalid base");
    CLIENT_ASSERT(index_reg <= REG_LAST_ENUM,
                  "opnd_create_base_disp_aarch64: invalid index");
    /* reg_id_t is now a ushort, but we can only accept low values */
    CLIENT_ASSERT_BITFIELD_TRUNCATE(REG_SPECIFIER_BITS, base_reg,
                                    "opnd_create_base_disp_aarch64: invalid base");
    CLIENT_ASSERT_BITFIELD_TRUNCATE(REG_SPECIFIER_BITS, index_reg,
                                    "opnd_create_base_disp_aarch64: invalid index");
    opnd.value.base_disp.base_reg = base_reg;
    opnd.value.base_disp.index_reg = index_reg;
    opnd.value.base_disp.pre_index = false;
    opnd_set_disp_helper(&opnd, disp);
    opnd.aux.flags = flags;
    if (!opnd_set_index_extend(&opnd, extend_type, scaled))
        CLIENT_ASSERT(false, "opnd_create_base_disp_aarch64: invalid extend type");
    return opnd;
}

/*
   opnd_uses_reg is now changed so that it does consider 8/16 bit
   register overlaps.  i think this change is OK and correct, but not
   sure.  as far as I'm aware, only my optimization stuff and the
   register stealing code (which is now not used, right?) relies on
   this code ==> but we now export it via CI API */

bool
opnd_uses_reg(opnd_t opnd, reg_id_t reg)
{
    if (reg == REG_NULL)
        return false;
    switch (opnd.kind) {
    case NULL_kind:
    case IMMED_INTEGER_kind:
    case IMMED_FLOAT_kind:
    case IMMED_DOUBLE_kind:
    case PC_kind:
    case FAR_PC_kind:
    case INSTR_kind:
    case FAR_INSTR_kind:
    case MEM_INSTR_kind: return false;

    case REG_kind: return (dr_reg_fixer[reg] == dr_reg_fixer[opnd_get_reg(opnd)]);

    case BASE_DISP_kind:
        return (dr_reg_fixer[reg] == dr_reg_fixer[opnd_get_base(opnd)] ||
                dr_reg_fixer[reg] == dr_reg_fixer[opnd_get_index(opnd)] ||
                dr_reg_fixer[reg] == dr_reg_fixer[opnd_get_segment(opnd)]);

#if defined(X64) || defined(ARM)
    case REL_ADDR_kind:
#endif
#ifdef X64
    case ABS_ADDR_kind:
        return (dr_reg_fixer[reg] == dr_reg_fixer[opnd_get_segment(opnd)]);
#endif

    default: CLIENT_ASSERT(false, "opnd_uses_reg: unknown opnd type"); return false;
    }
}

static uint
opnd_size_to_extend_amount(opnd_size_t size)
{
    switch (size) {
    default:
        ASSERT(false);
        /* fall-through */
    case OPSZ_1: return 0;
    case OPSZ_2: return 1;
    case OPSZ_4: return 2;
    case OPSZ_0: /* fall-through */
    case OPSZ_8: return 3;
    case OPSZ_16: return 4;
    }
}

dr_extend_type_t
opnd_get_index_extend(opnd_t opnd, OUT bool *scaled, OUT uint *amount)
{
    dr_extend_type_t extend = DR_EXTEND_UXTX;
    bool scaled_out = false;
    uint amount_out = 0;
    if (!opnd_is_base_disp(opnd))
        CLIENT_ASSERT(false, "opnd_get_index_shift called on invalid opnd type");
    else {
        extend = opnd.value.base_disp.extend_type;
        scaled_out = opnd.value.base_disp.scaled;
        if (scaled_out)
            amount_out = opnd_size_to_extend_amount(opnd_get_size(opnd));
    }
    if (scaled != NULL)
        *scaled = scaled_out;
    if (amount != NULL)
        *amount = amount_out;
    return extend;
}

/**
 * These flags describe how the index register in a memory reference is shifted
 * before being added to or subtracted from the base register.  They also describe
 * how a general source register is shifted before being used in its containing
 * instruction.
 */
typedef enum _dr_shift_type_t {
    DR_SHIFT_LSL, /**< Logical shift left. */
    DR_SHIFT_LSR, /**< Logical shift right. */
    DR_SHIFT_ASR, /**< Arithmetic shift right. */
    DR_SHIFT_ROR, /**< Rotate right. */
    /**
     * The register is rotated right by 1 bit, with the carry flag (rather than
     * bit 0) being shifted in to the most-significant bit.  (For shifts of
     * general source registers, if the instruction writes the condition codes,
     * bit 0 is then shifted into the carry flag: but for memory references bit
     * 0 is simply dropped.)
     * Only valid for shifts whose amount is stored in an immediate, not a register.
     */
    DR_SHIFT_RRX,
    /**
     * No shift.
     * Only valid for shifts whose amount is stored in an immediate, not a register.
     */
    DR_SHIFT_NONE,
} dr_shift_type_t;

/* NOTE: requires caller to be under PRESERVE_FLOATING_POINT_STATE */
float
opnd_get_immed_float(opnd_t opnd)
{
    CLIENT_ASSERT(opnd_is_immed_float(opnd),
                  "opnd_get_immed_float called on non-immed-float");
    /* note that manipulating floats is dangerous - see case 4360
     * this return shouldn't require any fp state, though
     */
    return opnd.value.immed_float;
}

DR_API
ushort
opnd_get_shift(opnd_t opnd)
{
    CLIENT_ASSERT(opnd_is_near_instr(opnd), "opnd_get_shift called on non-near-instr");
    return opnd.aux.shift;
}

enum {
    /* OPSZ_ constants not exposed to the user so ok to be shifted
     * by additions in the main enum.
     */
    OPSZ_1_of_4 = OPSZ_LAST, /* 8 bits, but can be part of a GPR register */
    OPSZ_2_of_4,             /* 16 bits, but can be part of a GPR register */
    OPSZ_1_of_8,             /* 8 bits, but can be part of an MMX register */
    OPSZ_2_of_8,             /* 16 bits, but can be part of MMX register */
    OPSZ_4_of_8,             /* 32 bits, but can be half of MMX register */
    OPSZ_1_of_16,            /* 8 bits, but can be part of XMM register */
    OPSZ_2_of_16,            /* 16 bits, but can be part of XMM register */
    OPSZ_4_of_16,            /* 32 bits, but can be part of XMM register */
    OPSZ_4_rex8_of_16,       /* 32 bits, 64 with rex.w, but can be part of XMM register */
    OPSZ_8_of_16,            /* 64 bits, but can be half of XMM register */
    OPSZ_12_of_16,           /* 96 bits: 3/4 of XMM */
    OPSZ_12_rex8_of_16,      /* 96 bits, or 64 with rex.w: 3/4 of XMM */
    OPSZ_14_of_16,           /* 112 bits; all but one word of XMM */
    OPSZ_15_of_16,           /* 120 bits: all but one byte of XMM */
    OPSZ_16_of_32,           /* 128 bits: half of YMM */
    OPSZ_half_16_vex32,      /* half of 128 bits (XMM or memory);
                              * if vex.L then is half of 256 bits (YMM or memory).
                              */
    OPSZ_half_16_vex32_evex64,    /* 64 bits, but can be half of XMM register;
                                   * if evex.L then is 256 bits (YMM or memory);
                                   * if evex.L' then is 512 bits (ZMM or memory).
                                   */
    OPSZ_quarter_16_vex32,        /* quarter of 128 bits (XMM or memory);
                                   * if vex.L then is quarter of 256 bits (YMM or memory).
                                   */
    OPSZ_quarter_16_vex32_evex64, /* quarter of 128 bits (XMM or memory);
                                   * if evex.L then is quarter of 256 bits (YMM or
                                   * memory);
                                   * if evex.L' then is quarter of 512 bits (ZMM
                                   * or memory).
                                   */
    OPSZ_eighth_16_vex32,         /* eighth of 128 bits (XMM or memory);
                                   * if vex.L then is eighth of 256 bits (YMM or memory).
                                   */
    OPSZ_eighth_16_vex32_evex64,  /* eighth of 128 bits (XMM or memory);
                                   * if evex.L then is eighth of 256 bits (YMM or
                                   * memory);
                                   * if evex.L' then is eighth of 512 bits (ZMM
                                   * or memory).
                                   */
    OPSZ_SUBREG_START = OPSZ_1_of_4,
    OPSZ_SUBREG_END = OPSZ_eighth_16_vex32_evex64,
    OPSZ_LAST_ENUM, /* note last is NOT inclusive */
};


uint
opnd_size_in_bytes(opnd_size_t size)
{
    CLIENT_ASSERT(size >= OPSZ_FIRST, "opnd_size_in_bytes: invalid size");
    switch (size) {
    case OPSZ_0: return 0;
    case OPSZ_1:
    case OPSZ_1_reg4: /* mem size */
    case OPSZ_1_of_4:
    case OPSZ_1_of_8:
    case OPSZ_1_of_16:
    case OPSZ_1b: /* round up */
    case OPSZ_2b:
    case OPSZ_3b:
    case OPSZ_4b:
    case OPSZ_5b:
    case OPSZ_6b:
    case OPSZ_7b: return 1;
    case OPSZ_2_of_4:
    case OPSZ_2_of_8:
    case OPSZ_2_of_16:
    case OPSZ_2_short1: /* default size */
    case OPSZ_2:
    case OPSZ_2_reg4: /* mem size */
    case OPSZ_9b:     /* round up */
    case OPSZ_10b:
    case OPSZ_11b:
    case OPSZ_12b:
    case OPSZ_eighth_16_vex32:
    case OPSZ_eighth_16_vex32_evex64: return 2;
    case OPSZ_20b: /* round up */
    case OPSZ_3: return 3;
    case OPSZ_4_of_8:
    case OPSZ_4_of_16:
    case OPSZ_4_rex8_of_16:
    case OPSZ_4_short2: /* default size */
#ifndef X64
    case OPSZ_4x8:           /* default size */
    case OPSZ_4x8_short2:    /* default size */
    case OPSZ_4x8_short2xi8: /* default size */
#endif
    case OPSZ_4_short2xi4:   /* default size */
    case OPSZ_4_rex8_short2: /* default size */
    case OPSZ_4_rex8:
    case OPSZ_4:
    case OPSZ_4_reg16: /* mem size */
    case OPSZ_25b:     /* round up */
    case OPSZ_quarter_16_vex32:
    case OPSZ_quarter_16_vex32_evex64: return 4;
    case OPSZ_6_irex10_short4: /* default size */
    case OPSZ_6: return 6;
    case OPSZ_8_of_16:
    case OPSZ_half_16_vex32:
    case OPSZ_8_short2:
    case OPSZ_8_short4:
    case OPSZ_8:
#ifdef X64
    case OPSZ_4x8:           /* default size */
    case OPSZ_4x8_short2:    /* default size */
    case OPSZ_4x8_short2xi8: /* default size */
#endif
    case OPSZ_8_rex16:        /* default size */
    case OPSZ_8_rex16_short4: /* default size */
#ifndef X64
    case OPSZ_8x16: /* default size */
#endif
        return 8;
    case OPSZ_16:
    case OPSZ_16_vex32:
    case OPSZ_16_of_32:
    case OPSZ_16_vex32_evex64:
#ifdef X64
    case OPSZ_8x16: /* default size */
#endif
        return 16;
    case OPSZ_vex32_evex64: return 32;
    case OPSZ_6x10:
        /* table base + limit; w/ addr16, different format, but same total footprint */
        return IF_X64_ELSE(6, 10);
    case OPSZ_10: return 10;
    case OPSZ_12:
    case OPSZ_12_of_16:
    case OPSZ_12_rex8_of_16:
    case OPSZ_12_rex40_short6: /* default size */ return 12;
    case OPSZ_14_of_16:
    case OPSZ_14: return 14;
    case OPSZ_15_of_16:
    case OPSZ_15: return 15;
    case OPSZ_20: return 20;
    case OPSZ_24: return 24;
    case OPSZ_28_short14: /* default size */
    case OPSZ_28: return 28;
    case OPSZ_32:
    case OPSZ_32_short16: /* default size */ return 32;
    case OPSZ_36: return 36;
    case OPSZ_40: return 40;
    case OPSZ_44: return 44;
    case OPSZ_48: return 48;
    case OPSZ_52: return 52;
    case OPSZ_56: return 56;
    case OPSZ_60: return 60;
    case OPSZ_64: return 64;
    case OPSZ_68: return 68;
    case OPSZ_72: return 72;
    case OPSZ_76: return 76;
    case OPSZ_80: return 80;
    case OPSZ_84: return 84;
    case OPSZ_88: return 88;
    case OPSZ_92: return 92;
    case OPSZ_94: return 94;
    case OPSZ_96: return 96;
    case OPSZ_100: return 100;
    case OPSZ_104: return 104;
    case OPSZ_108_short94: /* default size */
    case OPSZ_108: return 108;
    case OPSZ_112: return 112;
    case OPSZ_116: return 116;
    case OPSZ_120: return 120;
    case OPSZ_124: return 124;
    case OPSZ_128: return 128;
    case OPSZ_512: return 512;
    case OPSZ_VAR_REGLIST: return 0; /* varies to match reglist operand */
    case OPSZ_xsave:
        return 0; /* > 512 bytes: client to use drutil_opnd_mem_size_in_bytes */
    default: CLIENT_ASSERT(false, "opnd_size_in_bytes: invalid opnd type"); return 0;
    }
}

DR_API
uint
opnd_size_in_bits(opnd_size_t size)
{
    switch (size) {
    case OPSZ_1b: return 1;
    case OPSZ_2b: return 2;
    case OPSZ_3b: return 3;
    case OPSZ_4b: return 4;
    case OPSZ_5b: return 5;
    case OPSZ_6b: return 6;
    case OPSZ_7b: return 7;
    case OPSZ_9b: return 9;
    case OPSZ_10b: return 10;
    case OPSZ_11b: return 11;
    case OPSZ_12b: return 12;
    case OPSZ_20b: return 20;
    case OPSZ_25b: return 25;
    default: return opnd_size_in_bytes(size) * 8;
    }
}

opnd_t
opnd_create_immed_double(double i)
{
    opnd_t opnd;
    opnd.kind = IMMED_DOUBLE_kind;
    /* Note that manipulating floats and doubles by copying in this way can
     * result in using FP load/store instructions which can trigger any pending
     * FP exception (i#386).
     */
    opnd.value.immed_double = i;
    /* currently only used for implicit constants that have no size */
    opnd.size = OPSZ_0;
    return opnd;
}

opnd_t
opnd_create_instr_ex(instr_t *instr, opnd_size_t size, ushort shift)
{
    opnd_t opnd;
    opnd.kind = INSTR_kind;
    opnd.value.instr = instr;
    opnd.aux.shift = shift;
    opnd.size = size;
    return opnd;
}

opnd_t
opnd_create_instr(instr_t *instr)
{
    return opnd_create_instr_ex(instr, OPSZ_PTR, 0);
}

double
opnd_get_immed_double(opnd_t opnd)
{
    CLIENT_ASSERT(opnd_is_immed_double(opnd),
                  "opnd_get_immed_double called on non-immed-float");
    return opnd.value.immed_double;
}

instr_t *
instr_create()
{
    instr_t *instr = (instr_t *)heap_alloc(sizeof(instr_t) HEAPACCT(ACCT_IR));
    /* everything initializes to 0, even flags, to indicate
     * an uninitialized instruction */
    memset((void *)instr, 0, sizeof(instr_t));
    instr_set_isa_mode(instr, DR_ISA_AMD64);
    return instr;
}

instr_t *
instr_build(int opcode, int instr_num_dsts, int instr_num_srcs)
{
    instr_t *instr = instr_create();
    instr_set_opcode(instr, opcode);
    instr_set_num_opnds(instr, instr_num_dsts, instr_num_srcs);
    return instr;
}

instr_t *
instr_create_1dst_3src(int opcode, opnd_t dst, opnd_t src1, opnd_t src2,
                       opnd_t src3)
{
    instr_t *in = instr_build(opcode, 1, 3);
    instr_set_dst(in, 0, dst);
    instr_set_src(in, 0, src1);
    instr_set_src(in, 1, src2);
    instr_set_src(in, 2, src3);
    return in;
}

instr_t *
instr_create_1dst_1src(int opcode, opnd_t dst, opnd_t src)
{
    instr_t *in = instr_build(opcode, 1, 1);
    instr_set_dst(in, 0, dst);
    instr_set_src(in, 0, src);
    return in;
}

instr_t *
instr_create_1dst_4src(int opcode, opnd_t dst, opnd_t src1, opnd_t src2,
                       opnd_t src3, opnd_t src4)
{
    instr_t *in = instr_build(opcode, 1, 4);
    instr_set_dst(in, 0, dst);
    instr_set_src(in, 0, src1);
    instr_set_src(in, 1, src2);
    instr_set_src(in, 2, src3);
    instr_set_src(in, 3, src4);
    return in;
}

instr_t *
instr_create_0dst_0src(int opcode)
{
    instr_t *in = instr_build(opcode, 0, 0);
    return in;
}

instr_t *
instr_create_0dst_1src(int opcode, opnd_t src)
{
    instr_t *in = instr_build(opcode, 0, 1);
    instr_set_src(in, 0, src);
    return in;
}

instr_t *
instr_create_1dst_2src(int opcode, opnd_t dst, opnd_t src1, opnd_t src2)
{
    instr_t *in = instr_build(opcode, 1, 2);
    instr_set_dst(in, 0, dst);
    instr_set_src(in, 0, src1);
    instr_set_src(in, 1, src2);
    return in;
}

instr_t *
instr_create_2dst_1src(int opcode, opnd_t dst1, opnd_t dst2, opnd_t src)
{
    instr_t *in = instr_build(opcode, 2, 1);
    instr_set_dst(in, 0, dst1);
    instr_set_dst(in, 1, dst2);
    instr_set_src(in, 0, src);
    return in;
}

#define OPND_CREATE_INT(val) OPND_CREATE_INTPTR(val)

#define OPND_CREATE_LSL() opnd_add_flags(OPND_CREATE_INT(DR_SHIFT_LSL), DR_OPND_IS_SHIFT)

/** Create a zero register operand of the same size as reg. */
#define OPND_CREATE_ZR(reg) \
    opnd_create_reg(opnd_get_size(reg) == OPSZ_4 ? DR_REG_WZR : DR_REG_XZR)

#define INSTR_CREATE_bl(pc) \
    instr_create_1dst_1src(OP_bl, opnd_create_reg(DR_REG_X30), (pc))

#define INSTR_CREATE_ret(Rn) instr_create_0dst_1src(OP_ret, (Rn))
// sub sp, str reg from stack, bl func, ldr reg from stack, add sp
#define INSTR_CREATE_str(mem, rt) instr_create_1dst_1src(OP_str, mem, rt)
#define INSTR_CREATE_ldr(Rd, mem) instr_create_1dst_1src(OP_ldr, (Rd), (mem))
#define INSTR_CREATE_sub_extend(rd, rn, rm, ext, exa)                             \
    instr_create_1dst_4src(OP_sub, rd, rn,                                        \
                           opnd_create_reg_ex(opnd_get_reg(rm), 0, DR_OPND_EXTENDED), \
                           opnd_add_flags(ext, DR_OPND_IS_EXTEND), exa)
#define INSTR_CREATE_sub_shift(rd, rn, rm_or_imm, sht, sha)                \
    opnd_is_reg(rm_or_imm)                                                     \
        ? instr_create_1dst_4src(                                              \
              OP_sub, (rd), (rn),                                        \
              opnd_create_reg_ex(opnd_get_reg(rm_or_imm), 0, DR_OPND_SHIFTED), \
              opnd_add_flags((sht), DR_OPND_IS_SHIFT), (sha))                  \
        : instr_create_1dst_4src(OP_sub, (rd), (rn), (rm_or_imm), (sht), (sha))
#define INSTR_CREATE_sub(rd, rn, rm_or_imm)                                         \
    opnd_is_reg(rm_or_imm)                                                              \
        ? /* _extend supports sp in rn, so prefer it, but it does not support imm. */   \
        INSTR_CREATE_sub_extend(rd, rn, rm_or_imm, OPND_CREATE_INT(DR_EXTEND_UXTX), \
                                OPND_CREATE_INT(0))                                     \
        : INSTR_CREATE_sub_shift(rd, rn, rm_or_imm, OPND_CREATE_LSL(),              \
                                 OPND_CREATE_INT(0))

#define INSTR_CREATE_add_extend(rd, rn, rm, ext, exa)                             \
    instr_create_1dst_4src(OP_add, rd, rn,                                        \
                           opnd_create_reg_ex(opnd_get_reg(rm), 0, DR_OPND_EXTENDED), \
                           opnd_add_flags(ext, DR_OPND_IS_EXTEND), exa)
#define INSTR_CREATE_add_shift(rd, rn, rm_or_imm, sht, sha)                \
    opnd_is_reg(rm_or_imm)                                                     \
        ? instr_create_1dst_4src(                                              \
              OP_add, (rd), (rn),                                        \
              opnd_create_reg_ex(opnd_get_reg(rm_or_imm), 0, DR_OPND_SHIFTED), \
              opnd_add_flags((sht), DR_OPND_IS_SHIFT), (sha))                  \
        : instr_create_1dst_4src(OP_add, (rd), (rn), (rm_or_imm), (sht), (sha))
#define INSTR_CREATE_add(rd, rn, rm_or_imm)                                         \
    opnd_is_reg(rm_or_imm)                                                              \
        ? /* _extend supports sp in rn, so prefer it, but it does not support imm. */   \
        INSTR_CREATE_add_extend(rd, rn, rm_or_imm, OPND_CREATE_INT(DR_EXTEND_UXTX), \
                                OPND_CREATE_INT(0))                                     \
        : INSTR_CREATE_add_shift(rd, rn, rm_or_imm, OPND_CREATE_LSL(),              \
                                 OPND_CREATE_INT(0))

#define INSTR_CREATE_nop() instr_create_0dst_0src(OP_nop)

#define INSTR_CREATE_and_shift(rd, rn, rm, sht, sha)                             \
    instr_create_1dst_4src(OP_and, (rd), (rn),                                 \
                           opnd_create_reg_ex(opnd_get_reg(rm), 0, DR_OPND_SHIFTED), \
                           opnd_add_flags((sht), DR_OPND_IS_SHIFT), (sha))

/**
 * Creates an AND instruction with one output and two inputs.
 * \param rd   The output register.
 * \param rn   The first input register.
 * \param rm_or_imm   The second input register or immediate.
 */
#define INSTR_CREATE_and(rd, rn, rm_or_imm)                             \
    (opnd_is_immed(rm_or_imm)                                               \
         ? instr_create_1dst_2src(OP_and, (rd), (rn), (rm_or_imm))    \
         : INSTR_CREATE_and_shift(rd, rn, rm_or_imm, OPND_CREATE_LSL(), \
                                  OPND_CREATE_INT(0)))

#define INSTR_CREATE_label() instr_create_0dst_0src(OP_LABEL)

#define INSTR_CREATE_br(xn) instr_create_0dst_1src(OP_br, (xn))
#define INSTR_CREATE_blr(xn) \
    instr_create_1dst_1src(OP_blr, opnd_create_reg(DR_REG_X30), (xn))

#define INSTR_CREATE_movk(rt, imm16, lsl) \
    instr_create_1dst_4src(OP_movk, rt, rt, imm16, OPND_CREATE_LSL(), lsl)
#define INSTR_CREATE_movz(rt, imm16, lsl) \
    instr_create_1dst_3src(OP_movz, rt, imm16, OPND_CREATE_LSL(), lsl)

#define INSTR_CREATE_adrp(rt, imm) instr_create_1dst_1src(OP_adrp, rt, imm)

#define INSTR_CREATE_stp(mem, rt1, rt2) \
    instr_create_1dst_2src(OP_stp, mem, rt1, rt2)

#define INSTR_CREATE_ldp(rt1, rt2, mem) \
    instr_create_2dst_1src(OP_ldp, rt1, rt2, mem)

#define INSTR_CREATE_mrs(Xt, sysreg) \
    instr_create_1dst_1src(OP_mrs, (Xt), (sysreg))
#define INSTR_CREATE_msr(sysreg, Xt) \
    instr_create_1dst_1src(OP_msr, (sysreg), (Xt))

/**
 * Set the predication value for an instruction.
 */
#define INSTR_PRED(instr_ptr, pred) instr_set_predicate((instr_ptr), (pred))

/**
 * This platform-independent macro creates an instr_t for a conditional
 * branch instruction that branches if the previously-set condition codes
 * indicate the condition indicated by \p pred.
 * \param dc  The void * dcontext used to allocate memory for the instr_t.
 * \param pred  The #dr_pred_type_t condition to match.
 * \param t   The opnd_t target operand for the instruction, which can be
 * either a pc (opnd_create_pc)()) or an instr_t (opnd_create_instr()).
 * Be sure to ensure that the limited reach of this short branch will reach
 * the target (a pc operand is not suitable for most uses unless you know
 * precisely where this instruction will be encoded).
 */
#define XINST_CREATE_jump_cond(pred, t) \
    (INSTR_PRED(INSTR_CREATE_bcond((t)), (pred)))

/**
 * This macro creates an instr_t for a conditional branch instruction. The condition
 * can be set using INSTR_PRED macro.
 * \param dc The void * dcontext used to allocate memory for the instr_t.
 * \param pc The opnd_t target operand containing the program counter to jump to.
 */
#define INSTR_CREATE_bcond(pc) instr_create_0dst_1src(OP_bcond, (pc))

#define INSTR_CREATE_subs_shift(rd, rn, rm_or_imm, sht, sha)               \
    opnd_is_reg(rm_or_imm)                                                     \
        ? instr_create_1dst_4src(                                              \
              OP_subs, (rd), (rn),                                       \
              opnd_create_reg_ex(opnd_get_reg(rm_or_imm), 0, DR_OPND_SHIFTED), \
              opnd_add_flags((sht), DR_OPND_IS_SHIFT), (sha))                  \
        : instr_create_1dst_4src(OP_subs, (rd), (rn), (rm_or_imm), (sht), (sha))

#define INSTR_CREATE_subs(rd, rn, rm_or_imm) \
    INSTR_CREATE_subs_shift(rd, rn, rm_or_imm, OPND_CREATE_LSL(), OPND_CREATE_INT(0))

/**
 * This platform-independent macro creates an instr_t for a comparison
 * instruction.
 * \param s1  The opnd_t explicit source operand for the instruction.
 * \param s2  The opnd_t explicit source operand for the instruction.
 */
#define XINST_CREATE_cmp(s1, s2) INSTR_CREATE_cmp(s1, s2)

#define INSTR_CREATE_cmp(rn, rm_or_imm) \
    INSTR_CREATE_subs(OPND_CREATE_ZR(rn), rn, rm_or_imm)

#define OPND_CREATE_INT16(val) opnd_create_immed_int((ptr_int_t)(val), OPSZ_2)

#define OPND_CREATE_INT8(val) opnd_create_immed_int((ptr_int_t)(val), OPSZ_1)

#define OPND_CREATE_ABSMEM(addr, size) opnd_create_rel_addr(addr, size)

#endif /* INSTR_OPND_API_H */
