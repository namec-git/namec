/****************************************************************************
**
** Copyright (C) 2007-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/tglobal.h#101 $$Change: 2353 $
** $Date: 2018/02/15 $$Author: bbarber $
**
****************************************************************************/

#ifndef tglobal_H
#define tglobal_H

#include <QtCore/QGlobalStatic>
#include <QtCore/QPair>
#include <QtCore/QList>

//! Use tglobal.h for macros and symbols defined by tglobal.cpp (none so far)
//! See thidcore/tnamespace.h for global constants, symbols, and typedefs in the Thesa namespace
//! Modeled on corelib/global/qglobal.h and corelib/global/qnamespace.h
namespace Thesa {

#//!\name Qt Macros

/*** Qt macros used by Thesa

// Assert condition true.  See also THID_ASSERT below
    Q_ASSERT(item_exists();
    Q_ASSERT_X(nextnode == curState->curnode->elements().last(), "RA_APP_HOLD", "holdnode copied incorrectlhy");

// Define type-safe flags
Q_DECLARE_OPERATORS_FOR_FLAGS(Flags);
Q_DECLARE_FLAGS(Flags,Flag);

// Define a custom type for more efficient layout of QList etc.
Q_DECLARE_TYPEINFO(QTextHtmlParserNode, Q_MOVABLE_TYPE);

// Disable copy constructor and assignment
private:
    Q_DISABLE_COPY(RoadEventPool)
public:

// Initialize resources at start of main()
Q_INIT_RESOURCE(thid);

// Declare interfaces for Qt plugins
    Q_INTERFACES(QTextObjectInterface)

// Mark an inherited QOBJECT class for moc.  Document the object's parent class
    Q_OBJECT  //! Owned by Thid
    // Use QPointer for field references to an unowned QObject, automatically sets to NULL on deconstruct

// Mark signal and slot declarations for a Q_OBJECT class
public Q_SIGNALS:
public Q_SLOTS:

// Avoid warnings about an unused parameter or variable
    Q_UNUSED(flags)

*/

#//!\name Macros
#define TR_(msg) QT_TRANSLATE_NOOP(TR_CONTEXT, msg)

//! Catch all errors from "source", two error codes, and execute 'instructions;' or '/* do nothing */'
//!\todo How to report non-std::exception errors such as floating point errors
//!<\todo Should catch and report errors instead of hide in log, e.g., ThMainWindow::onSaveAll -- also document detail2 logging
#define TH_CATCH_ERRORS(errorCode, errorCode2, source, instructions) \
    catch(const ThError &e){ \
       ThError::warn(e); \
       {instructions; } \
    }catch(const std::exception &e){ \
       ThWarn((errorCode), "%s: exception thrown: '%s'", 0,0, (source), e.what()); \
       {instructions; } \
    }catch(...){ \
       ThWarn((errorCode2), "%s: hardware exception thrown", (source)); \
       {instructions; } \
   }

//! Define comparison operators in terms of == and <
#define TH_DECLARE_COMPARISON_OPERATORS(TYPE) \
    bool            operator!=(const TYPE &other) const { return !(*this==other); } \
    bool            operator>=(const TYPE &other) const { return !(*this<other); } \
    bool            operator>(const TYPE &other) const { return other<*this; } \
    bool            operator<=(const TYPE &other) const { return !(other<*this); } \
    //TH_DECLARE_COMPARISON_OPERATORS

//! Based on qiterator.h::Q_DECLARE_ASSOCIATIVE_ITERATOR
//! [dec'12] Not used requires const_iterator over a single type
#define TH_DECLARE_ASSOCIATIVE_ITERATOR(C) \
    \
    template <class Key, class T> \
class Th##C##Iterator \
{ \
    typedef typename Th##C<Key,T>::const_iterator const_iterator; \
    typedef const_iterator Item; \
    Th##C<Key,T> c; \
    const_iterator i, n; \
    inline bool item_exists() const { return n != c.constEnd(); } \
public: \
    inline Th##C##Iterator(const Th##C<Key,T> &container) \
    : c(container), i(c.constBegin()), n(c.constEnd()) {} \
    inline Th##C##Iterator &operator=(const Th##C<Key,T> &container) \
{ c = container; i = c.constBegin(); n = c.constEnd(); return *this; } \
    inline void toFront() { i = c.constBegin(); n = c.constEnd(); } \
    inline void toBack() { i = c.constEnd(); n = c.constEnd(); } \
    inline bool hasNext() const { return i != c.constEnd(); } \
    inline Item next() { n = i++; return n; } \
    inline Item peekNext() const { return i; } \
    inline bool hasPrevious() const { return i != c.constBegin(); } \
    inline Item previous() { n = --i; return n; } \
    inline Item peekPrevious() const { const_iterator p = i; return --p; } \
    inline const T &value() const { Q_ASSERT(item_exists()); return *n; } \
    inline const Key &key() const { Q_ASSERT(item_exists()); return n.key(); } \
    inline bool findNext(const T &t) \
{ while ((n = i) != c.constEnd()) if(*i++ == t) return true; return false; } \
    inline bool findPrevious(const T &t) \
{ while (i != c.constBegin()) if(*(n = --i) == t) return true; \
    n = c.constEnd(); return false; } \
};


#ifndef THID_1
#define THID_ASSERT assert
#endif

}//namespace Thesa

#endif tglobal_H
