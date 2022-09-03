#define ACG_EMPTY()
#define ACG_DEFER(ID) ID ACG_EMPTY() // (ARGS)
#define ACG_CONSUME(...)
#define ACG_EVAL(...) __VA_ARGS__

#define JOIN_2(A, B) A ## B
#define JOIN_3(A, B, C) A ## B ## C
#define JOIN(A, B) JOIN_2(A, B)

#define ACG_IF___ELSE(...) /* CONSUME */ ACG_EVAL
#define ACG_IF_0_ELSE(...) /* CONSUME */ ACG_EVAL
#define ACG_IF_1_ELSE(...)  __VA_ARGS__  ACG_CONSUME
#define IF_01_ELSE(COND) JOIN_3(ACG_IF_, COND, _ELSE) // (THEN) (ELSE)

#define ACG_IF__(...) /* CONSUME */
#define ACG_IF_0(...) /* CONSUME */
#define ACG_IF_1(...)  __VA_ARGS__
#define IF_01(COND) JOIN(ACG_IF_, COND) // (THEN)

#define FST(FST, ...)      FST
#define SND(___, SND, ...) SND

#define ACG_IS_ONE(...) SND(__VA_ARGS__, 0)
#define ACG_ONE $, 1

#define ACG_NOT__ ACG_ONE
#define ACG_NOT_0 ACG_ONE
#define NOT(_) ACG_IS_ONE(JOIN(ACG_NOT_, _))
#define CAST_BOOL(_) NOT(NOT(_))

#define IF_ELSE(COND) IF_01_ELSE(CAST_BOOL(COND)) // (THEN) (ELSE)
#define IF(COND) IF_01(CAST_BOOL(COND)) // (THEN)

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
