#ifndef QASSERT_H
#define QASSERT_H

#ifdef Q_NASSERT /* Q_NASSERT defined--assertion checking disabled */

    /* provide dummy (empty) definitions that don't generate any code... */
    #define Q_DEFINE_THIS_FILE
    #define Q_DEFINE_THIS_MODULE(name_)
    #define Q_ASSERT(test_)             ((void)0)
    #define Q_ASSERT_ID(id_, test_)     ((void)0)
    #define Q_ALLEGE(test_)             ((void)(test_))
    #define Q_ALLEGE_ID(id_, test_)     ((void)(test_))
    #define Q_ERROR()                   ((void)0)
    #define Q_ERROR_ID(id_)             ((void)0)

#else  /* Q_NASSERT not defined--assertion checking enabled */

#ifndef QP_VERSION /* is quassert.h used outside QP? */

/* provide typedefs so that qassert.h could be used "standalone"... */

typedef char char_t;

typedef int int_t;

#ifndef Q_ROM /* if NOT defined, provide the default definition */
#define Q_ROM
#endif

#endif

#define Q_DEFINE_THIS_FILE \
        static char_t const Q_ROM Q_this_module_[] = __FILE__;

#define Q_DEFINE_THIS_MODULE(name_) \
        static char_t const Q_ROM Q_this_module_[] = name_;

#define Q_ASSERT(test_) ((test_) \
        ? (void)0 : Q_onAssert(&Q_this_module_[0], (int_t)__LINE__))

#define Q_ASSERT_ID(id_, test_) ((test_) \
        ? (void)0 : Q_onAssert(&Q_this_module_[0], (int_t)(id_)))

#define Q_ALLEGE(test_)    Q_ASSERT(test_)

#define Q_ALLEGE_ID(id_, test_) Q_ASSERT_ID((id_), (test_))

#define Q_ERROR() \
        Q_onAssert(&Q_this_module_[0], (int_t)__LINE__)

#define Q_ERROR_ID(id_) \
        Q_onAssert(&Q_this_module_[0], (int_t)(id_))

#endif /* Q_NASSERT */

/****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef Q_NORETURN
#define Q_NORETURN    void
#endif /*  Q_NORETURN */

    Q_NORETURN Q_onAssert(char_t const Q_ROM *const module,
            int_t const location);

#ifdef __cplusplus
    }
#endif

#define Q_REQUIRE(test_)         Q_ASSERT(test_)

#define Q_REQUIRE_ID(id_, test_) Q_ASSERT_ID((id_), (test_))

#define Q_ENSURE(test_) Q_ASSERT(test_)

#define Q_ENSURE_ID(id_, test_) Q_ASSERT_ID((id_), (test_))

#define Q_INVARIANT(test_) Q_ASSERT(test_)

#define Q_INVARIANT_ID(id_, test_) Q_ASSERT_ID((id_), (test_))

#define Q_ASSERT_STATIC(test_) \
    extern int_t Q_assert_static[(test_) ? 1 : -1]

#define Q_ASSERT_COMPILE(test_) Q_ASSERT_STATIC(test_)

#define Q_DIM(array_) (sizeof(array_) / sizeof((array_)[0]))

#endif /* QASSERT_H */
