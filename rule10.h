// Unless stated otherwise,
// conditional syntax is as follows:
//   IF      (COND) (THEN)
//   IF_ELSE (COND) (THEN) (ELSE)

#define ARG_NULL ()

/* Simple conditional:
 * COND must resolve to either 'false'
 * (accepted values are '0' or '_')
 * or '1' ('true')
 */

#define IF_01(COND) JOIN(ACG_IF_, COND) // (THEN)
#define IF_01_ELSE(COND) JOIN_3(ACG_IF_, COND, _ELSE) // (THEN) (ELSE)

/* More powerful conditional:
 * COND must resolve to either 'false' (see above)
 * or any valid identifier ('true' -- an 'implicit bool cast'),
 * including spaces and some special characters
 */

#define IF_ELSE(COND) IF_01_ELSE(CAST_BOOL(COND)) // (THEN) (ELSE)
#define IF(COND) IF_01(CAST_BOOL(COND)) // (THEN)

/* Even more powerful conditional:
 * COND may be just about any input ('true'),
 * as long as it is not wrapped in parens,
 * or 'null' (ARG_NULL)
 */

#define IF_EXISTS(COND) IF_NOT_ARG_NULL(COND) // (THEN)
#define IF_EXISTS_ELSE(COND) IF_NOT_ARG_NULL_ELSE(COND) // (THEN) (ELSE)

/* Take a nullable value and 'unwrap' it in place;
 * if it is indeed null, return empty
 */

#define TRY_EMPLACE(ARG) IF_EXISTS(ARG) (ARG)
#define TRY_EMPLACE_WITH(ARG, WITH) IF_EXISTS(ARG) (ARG WITH)
#define TRY_EMPLACE_WITH_COMMA(ARG) IF_EXISTS(ARG) (ARG ,)

/* Additional null checks */

#define IF_ARG_NULL(COND) IF_01(IS_WRAPPED_EMPTY(COND)) // (THEN)
#define IF_ARG_NULL_ELSE(COND) \
	IF_01_ELSE(IS_WRAPPED_EMPTY(COND)) // (THEN) (ELSE)

#define IF_NOT_ARG_NULL(COND) IF_01(NOT(IS_WRAPPED_EMPTY(COND))) // (THEN)
#define IF_NOT_ARG_NULL_ELSE(COND) \
	IF_01_ELSE(NOT(IS_WRAPPED_EMPTY(COND))) // (THEN) (ELSE)

/* Utility */

#define EMPTY()
#define IS_EMPTY(ARG) ACG_IS_ONE(JOIN_3(ACG_NOT_, _, ARG))

#define FST(FST, ...)      FST
#define SND(___, SND, ...) SND

#define NOT(_) ACG_IS_ONE(JOIN(ACG_NOT_, _))
#define CAST_BOOL(_) NOT(NOT(_))

#define AND(A, B) JOIN_3(ACG_AND_, A, B)

#define JOIN(A, B) JOIN_2(A, B)
#define JOIN_2(A, B)    A ## B
#define JOIN_3(A, B, C) A ## B ## C

/* Paren 'wrapping' */

#define WRAP(...) (__VA_ARGS__)
#define UNWRAP(...) ACG_EVAL __VA_ARGS__

#define IS_WRAPPED(_) ACG_IS_ONE(ACG_ONE_CONSUME _)

#define TRY_UNWRAP(...) \
	IF_WRAPPED(__VA_ARGS__) (UNWRAP(__VA_ARGS__))

#define IF_WRAPPED(COND) \
	IF_01(IS_WRAPPED(COND)) // (THEN)
#define IF_WRAPPED_ELSE(COND) \
	IF_01_ELSE(IS_WRAPPED(COND)) // (THEN) (ELSE)

#define IF_UNWRAPPED(COND) \
	IF_01(NOT(IS_WRAPPED(COND))) // (THEN)
#define IF_UNWRAPPED_ELSE(COND) \
	IF_01_ELSE(NOT(IS_WRAPPED(COND))) // (THEN) (ELSE)

#define IS_WRAPPED_FALSE(_) \
	AND(IS_WRAPPED(_), NOT(UNWRAP(_)))
#define IS_WRAPPED_TRUE(_) \
	AND(IS_WRAPPED(_), CAST_BOOL(UNWRAP(_)))
#define IS_WRAPPED_EMPTY(_) \
	AND(IS_WRAPPED(_), IS_EMPTY(UNWRAP(_)))

/* &c */

#define PASTE_1(...) __VA_ARGS__
#define PASTE_2(...)  PASTE_1(__VA_ARGS__)  PASTE_1(__VA_ARGS__)
#define PASTE_3(...)  PASTE_2(__VA_ARGS__)  PASTE_1(__VA_ARGS__)
#define PASTE_4(...)  PASTE_2(__VA_ARGS__)  PASTE_2(__VA_ARGS__)
#define PASTE_5(...)  PASTE_4(__VA_ARGS__)  PASTE_1(__VA_ARGS__)
#define PASTE_6(...)  PASTE_4(__VA_ARGS__)  PASTE_2(__VA_ARGS__)
#define PASTE_7(...)  PASTE_4(__VA_ARGS__)  PASTE_3(__VA_ARGS__)
#define PASTE_8(...)  PASTE_4(__VA_ARGS__)  PASTE_4(__VA_ARGS__)
#define PASTE_9(...)  PASTE_8(__VA_ARGS__)  PASTE_1(__VA_ARGS__)
#define PASTE_10(...) PASTE_8(__VA_ARGS__)  PASTE_2(__VA_ARGS__)
#define PASTE_11(...) PASTE_8(__VA_ARGS__)  PASTE_3(__VA_ARGS__)
#define PASTE_12(...) PASTE_8(__VA_ARGS__)  PASTE_4(__VA_ARGS__)
#define PASTE_13(...) PASTE_8(__VA_ARGS__)  PASTE_5(__VA_ARGS__)
#define PASTE_14(...) PASTE_8(__VA_ARGS__)  PASTE_6(__VA_ARGS__)
#define PASTE_15(...) PASTE_8(__VA_ARGS__)  PASTE_7(__VA_ARGS__)
#define PASTE_16(...) PASTE_8(__VA_ARGS__)  PASTE_8(__VA_ARGS__)
#define PASTE_17(...) PASTE_16(__VA_ARGS__) PASTE_1(__VA_ARGS__)
#define PASTE_18(...) PASTE_16(__VA_ARGS__) PASTE_2(__VA_ARGS__)
#define PASTE_19(...) PASTE_16(__VA_ARGS__) PASTE_3(__VA_ARGS__)
#define PASTE_20(...) PASTE_16(__VA_ARGS__) PASTE_4(__VA_ARGS__)
#define PASTE_21(...) PASTE_16(__VA_ARGS__) PASTE_5(__VA_ARGS__)
#define PASTE_22(...) PASTE_16(__VA_ARGS__) PASTE_6(__VA_ARGS__)
#define PASTE_23(...) PASTE_16(__VA_ARGS__) PASTE_7(__VA_ARGS__)
#define PASTE_24(...) PASTE_16(__VA_ARGS__) PASTE_8(__VA_ARGS__)
#define PASTE_25(...) PASTE_16(__VA_ARGS__) PASTE_9(__VA_ARGS__)
#define PASTE_26(...) PASTE_16(__VA_ARGS__) PASTE_10(__VA_ARGS__)
#define PASTE_27(...) PASTE_16(__VA_ARGS__) PASTE_11(__VA_ARGS__)
#define PASTE_28(...) PASTE_16(__VA_ARGS__) PASTE_12(__VA_ARGS__)
#define PASTE_29(...) PASTE_16(__VA_ARGS__) PASTE_13(__VA_ARGS__)
#define PASTE_30(...) PASTE_16(__VA_ARGS__) PASTE_14(__VA_ARGS__)
#define PASTE_31(...) PASTE_16(__VA_ARGS__) PASTE_15(__VA_ARGS__)
#define PASTE_32(...) PASTE_16(__VA_ARGS__) PASTE_16(__VA_ARGS__)
#define PASTE_33(...) PASTE_32(__VA_ARGS__) PASTE_1(__VA_ARGS__)
#define PASTE_34(...) PASTE_32(__VA_ARGS__) PASTE_2(__VA_ARGS__)
#define PASTE_35(...) PASTE_32(__VA_ARGS__) PASTE_3(__VA_ARGS__)
#define PASTE_36(...) PASTE_32(__VA_ARGS__) PASTE_4(__VA_ARGS__)
#define PASTE_37(...) PASTE_32(__VA_ARGS__) PASTE_5(__VA_ARGS__)
#define PASTE_38(...) PASTE_32(__VA_ARGS__) PASTE_6(__VA_ARGS__)
#define PASTE_39(...) PASTE_32(__VA_ARGS__) PASTE_7(__VA_ARGS__)
#define PASTE_40(...) PASTE_32(__VA_ARGS__) PASTE_8(__VA_ARGS__)
#define PASTE_41(...) PASTE_32(__VA_ARGS__) PASTE_9(__VA_ARGS__)
#define PASTE_42(...) PASTE_32(__VA_ARGS__) PASTE_10(__VA_ARGS__)
#define PASTE_43(...) PASTE_32(__VA_ARGS__) PASTE_11(__VA_ARGS__)
#define PASTE_44(...) PASTE_32(__VA_ARGS__) PASTE_12(__VA_ARGS__)
#define PASTE_45(...) PASTE_32(__VA_ARGS__) PASTE_13(__VA_ARGS__)
#define PASTE_46(...) PASTE_32(__VA_ARGS__) PASTE_14(__VA_ARGS__)
#define PASTE_47(...) PASTE_32(__VA_ARGS__) PASTE_15(__VA_ARGS__)
#define PASTE_48(...) PASTE_32(__VA_ARGS__) PASTE_16(__VA_ARGS__)
#define PASTE_49(...) PASTE_32(__VA_ARGS__) PASTE_17(__VA_ARGS__)
#define PASTE_50(...) PASTE_32(__VA_ARGS__) PASTE_18(__VA_ARGS__)
#define PASTE_51(...) PASTE_32(__VA_ARGS__) PASTE_19(__VA_ARGS__)
#define PASTE_52(...) PASTE_32(__VA_ARGS__) PASTE_20(__VA_ARGS__)
#define PASTE_53(...) PASTE_32(__VA_ARGS__) PASTE_21(__VA_ARGS__)
#define PASTE_54(...) PASTE_32(__VA_ARGS__) PASTE_22(__VA_ARGS__)
#define PASTE_55(...) PASTE_32(__VA_ARGS__) PASTE_23(__VA_ARGS__)
#define PASTE_56(...) PASTE_32(__VA_ARGS__) PASTE_24(__VA_ARGS__)
#define PASTE_57(...) PASTE_32(__VA_ARGS__) PASTE_25(__VA_ARGS__)
#define PASTE_58(...) PASTE_32(__VA_ARGS__) PASTE_26(__VA_ARGS__)
#define PASTE_59(...) PASTE_32(__VA_ARGS__) PASTE_27(__VA_ARGS__)
#define PASTE_60(...) PASTE_32(__VA_ARGS__) PASTE_28(__VA_ARGS__)
#define PASTE_61(...) PASTE_32(__VA_ARGS__) PASTE_29(__VA_ARGS__)
#define PASTE_62(...) PASTE_32(__VA_ARGS__) PASTE_30(__VA_ARGS__)
#define PASTE_63(...) PASTE_32(__VA_ARGS__) PASTE_31(__VA_ARGS__)
#define PASTE_64(...) PASTE_32(__VA_ARGS__) PASTE_32(__VA_ARGS__)

/* Internal */

#define ACG_CONSUME(...)
#define ACG_EVAL(...) __VA_ARGS__

#define ACG_IF__(...) /* CONSUME */
#define ACG_IF_0(...) /* CONSUME */
#define ACG_IF_1(...)  __VA_ARGS__

#define ACG_IF___ELSE(...) /* CONSUME */ ACG_EVAL
#define ACG_IF_0_ELSE(...) /* CONSUME */ ACG_EVAL
#define ACG_IF_1_ELSE(...)  __VA_ARGS__  ACG_CONSUME

#define ACG_IS_ONE(...) SND(__VA_ARGS__, 0)
#define ACG_ONE $, 1
#define ACG_ONE_CONSUME(...) ACG_ONE

#define ACG_NOT__ ACG_ONE
#define ACG_NOT_0 ACG_ONE

#define ACG_AND_00 0
#define ACG_AND_01 0
#define ACG_AND_11 1
#define ACG_AND_10 0
