/****************************************************************************
**
** Copyright (C) 2015-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/tnamespace.h#54 $$Change: 2452 $
** $Date: 2018/07/31 $$Author: bbarber $
**
****************************************************************************/

#ifndef tnamespace_H
#define tnamespace_H

#include <QtCore/QGlobalStatic>
#include <QtCore/QList>
#include <QtCore/QPair>

//! Define symbols in the Thesa namespace for use by all modules
//! Use thidcore/tglobal.h for macros and for global symbols defined by tglobal.cpp (currently none)
//! Modeled on corelib/global/qglobal.h and corelib/global/qnamespace.h
namespace Thesa {

#//!\name typedef

typedef int             DepotLocalInt;           //!< DepotLocalId as an int.  Do not assume int
typedef int             NameIdentifierInt;      //! ThCode::NameIdentifier's
typedef short           OperandIdentifierInt;   //! ThCode::OperandIdentifier's
typedef quint64         ThDiskBlockInt;         //!< ThDiskBlockId as an int
typedef qint64          ThFileOffsetInt;        //!< Byte offset into a file.  Same as QIODevice::seek
typedef quint32         ThFlagsInt;             //!< Base type for flags.  Unsigned for high bit mask.
typedef quint32         ThidHashInt;            //!< ThidId, ThRevisionId, and title hash \see MaximumThidHashInt
typedef ThidHashInt     ThidIdHash;             //!< ThidId and ThRevisionId hash into a ThidBucket of ThDiskBlockId.  Same as ThRevisionId::SHA1ShortCount
typedef ThidHashInt     ThidTitleHash;          //!< Thid title hash into a ThidBucket
typedef QString         ThLocaleId;             //!< Locale (default 'C') \todo change to numeric id with various selection opportunities
typedef int             ThLocalInt;             //!< Index into a TextDocument local list (e.g., local_namespaces)
typedef int             ThNameInt;              //!< ThName as a ThTextDocument index
typedef int             ThProtobufInt;          //!< A protobuf index
typedef quint64         ThRevisionId64;         //!< 64-bit abbreviation of ThRevisionId, for missing_revisions 
typedef ThidIdHash      ThRevisionHash;         //!< 32-bit hash of a ThRevisionId, for ThidBucket
typedef quint64         ThSectorInt;            //!< Disk block sector address
typedef int             ThThidInt;              //!< ThidId as a ThTextDocument index
typedef quint32         ThUtcTime;              //!< Will be qint64 someday.  Same as ThDiskBlock::modifiedTime_t()
typedef quint32         ThUtcDelta;             //!< A time period.   Will be qint64 someday.
typedef quint32         ThXrefSignatureInt;     //!< Encoded xref signature

static const int        MaximumTimeBytes= 5;    //!< Allow room for one more ThUtcTime byte



//!<\todo Test compiling with ThUtcTime as qint64 or quint64
typedef quint16         ThPort;                 //!< Network port identifier
                                                //!< Undef value is '0'.  This differs from QDateTime::toTime_t().

typedef QPair<const char *, int> ThSubBytes;       //!< Start and count into a byte string
typedef QList<ThSubBytes> ThSubBytesList;

#//!\name Global constants \todo Use enum or static const int
#//!\todo Move ByteHighBit,etc. to RoadUtility?
#//! Warning: ~BlockSectorMask does not work in DevStudio 2005.  Convert to quint64 before complementing.

enum {
    Ascii_a= 0x61,
    HexNumber= 16,
    _K= 1024,
    _M= 1024*1024,

    NoHash=             0,      //!< Invalid ThidIdHash or ThidTitleHash
    NoIndex=            -1,            //!< Not an index value 0..n-1
    ErrorCount=         -1,         //!< Function returned an error instead of a count
    DaysInWeek=         7,
    DefaultYearCreated= 0,
    YearMax=            2100,          //!< Maximum year for ThDiskBlockRaw::yearCreated. \todo YearMax should be 50 years from today
    ReverseSort=        true,
};

enum boolConstants : bool {
    th_ALL=     true,       //!< For 'all' parameters
};

enum utcConstants : ThUtcTime {
    MinimumUtcTime= 0x01000000, //!< Require at least 4 bytes for ThidId::thidTimeSizes()
    MinimumCurrentTime= 631170000, //!< Minimum currentTime, 1990/01/01
    NoUtcTime= 0,  //!< If time not defined, use Jan 1, 1971 \todo Sets ThidData.text_modified_timed
    InvalidUtcTime= 0xffffffff, //!< Returned by QDateTime.toTime_t
    UtcYear0= 1970,           //!< Base year for ThDiskBlockRaw::yearCreated and ThUtcTime
    UtcYear1= 31536000,       //!< ThUtcTime for year 1, 1971-01-01 UTC
    UtcMaximumYear= UtcYear0+68+68,  //!< 2106
    UtcDay= 24*3600,       // 86400
    UtcWeek0= 4*UtcDay,  //!< First Monday in 1970 (UtcYear0), 345600 is Mon Jan  5 00:00:00 UTC 1970, while ThUtcTime 0 is Thurs Jan 1 00:00:00 UTC
    UtcWeek= 7*UtcDay,     //
    UtcWeek4= 4*UtcWeek,
    UtcDaysPerNormalYear= 365,  //!< Days in a non-leap year (182.5 days in a normal Month6)
    UtcNormalYear= UtcDaysPerNormalYear*UtcDay,   // 31536000.  Half year is July 2 12:00 UTC or July 1 12:00 for leap years
    UtcNormalMonth6= UtcNormalYear/2,   //  July 2 12:00 UTC.  Leap years it is June 1 12:00 UTC
};

enum charConstants : char {
    ArgSprintf= '%',     //!< Format character for QString::sprintf() and sprintf()
    MinimumPrintable= ' ', //!< Minimum isPrint() character
    UnicodeChar= '?',   //!< Default alternative char for Unicode in RTF (\uc1\uN?)
    NumberDelimiter= ':',     //!< Delimiter between numbers in a string (e.g., RoadUtility::toRectangle)
    LastXrefUnderscore= 'u',  //!< ThXrefSignature character for '_' as last character
    CommentChar= '/',    //!< "//" indicates a comment
    Comment2Char= ';',    //!< ";;" indicates a Thesa comment
    TupleChar= '@',     //!< First character of tuple format
    RtfChar= '{',       //!< First character of rtf format
    SearchExactChar= '"',  //!< "..." indicates exact matching, otherwise upper/lower, any suffix for 4+ char
    SearchNotChar= '!',  //!< A leading '!' indicates 'not' token
    SearchOrChar= '+',  //!< A leading '+' indicates 'or' token
    SearchKeepChar= '+',  //!< A leading '++' indicates 'keep' token
    SearchWildChar= '?',  //!< An embedded '?' indicates a one character wildcard
    UnderscoreAlpha= '_', //!< An underscore character is a treated as an alphabetic
    ThidLocationChar= '/', //!< ThidParse::LocationSeparator and ThidIdentifier::LocationThidPrefix
    TimeChar= 'T',      //!< A ThUtcTime is 'yyyymmddT...'
      TimeCharIndex= 8,
    XmlChar= '<',       //!< First character of XML elements
    XmlEndChar= '>',    //!< Last character of XML elements
    //QChar::ObjectReplacementCharacter -- drawn as ThTextObject (gap) or ThTextThidObject (thid).  Always at start of a QTextFragment (due to a new QTextFormat)
    //ThTextDocument::ThTextDocumentConstants  -- QTextDocument formating characters
};

enum ThMoveOperation : int { //!< Extends QTextCursor::MoveOperation \see ThTextCursor and ThTupleCursor
    PreviousField   = 100,      //!< Move to first position in previous ThField
    StartOfField    = 101,      //!< Move to first position in current ThField
    EndOfField      = 102,      //!< Move to last position in current ThField
    NextField       = 103,      //!< Move to first position in next ThField

    PreviousTuple   = 104,      //!< Move to first position in previous ThField
    StartOfTuple    = 105,      //!< Move to first position in current ThTuple
    EndOfTuple      = 106,      //!< Move to last position in current ThTuple
    NextTuple       = 107,      //!< Move to first position in next ThTuple

    StartOfParent   = 108,      //!< Move to first position of parent ThTuple

    FirstSibling    = 109,      //!< Move to first position of first sibling ThTuple, skips children
    PreviousSibling = 110,      //!< Move to first position of previous sibling ThTuple, skips children
    NextSibling     = 111,      //!< Move to first position of next sibling ThTuple, skips children
    LastSibling     = 112,       //!< Move to first position of last sibling ThTuple, skips children

    NextTextThid    = 150,      //!< Move to next ObjectReplacementCharacter marking a ThTextThid.
    PreviousTextThid = 151,     //!< Move to previous ObjectReplacementCharacter marking a ThTextThid
}; //ThMoveOperation

enum numericConstants : qint64 { // includes MinimumQint64 as an L constant, may cause a warning
    _G= 1024*1024*1024LL,
    MaximumQint64=  0x7fffffffffffffffL,
    MaximumQint32=  0x000000007fffffffL,
    MaximumQuint32= 0x00000000ffffffffL,
    MaximumQuint48= 0x0000ffffffffffffL,
    MinimumQint32=  0x0000000080000000L,
    MinimumQint64=  0x8000000000000000L,
    BitMask40=      0x000000ffffffffffL,
    UtcJulianYear0= 434534534534,
};
enum {
    MaximumInt= 0x7fffffff,  //!<\todo Should MaximumInt and MinimumInt be disallowed, assumes 32-bit
    MinimumInt= 0x80000000,
    MaximumQuint8= 0x0000000ff,  
    MaximumThidHashInt= 0xffffffff,
};
// For float/double constants use float.h FLT_MIN and FLT_MAX
static const quint32 AllBits32=    0xffffffff;
static const quint32 Bits7Mask=    0x0000007f;
static const quint32 Bits15Mask=   0x00007fff;
static const quint32 ByteMask=     0x000000ff;
static const quint32 Byte2Mask=    0x0000ffff;
static const quint32 Byte3Mask=    0x00ffffff; // 16777215
static const quint32 HexMask=      0x0000000f;
static const quint32 HighByteMask= 0xff000000;
static const quint32 HighNibbleMask= 0xf0000000;
static const quint32 HighWordMask= 0xffff0000;
static const quint32 IntHighBit=   0x80000000;
static const quint32 KeyModifierMask= 0xff000000;  //!< Bit mask for Qt::KeyboardModifier
static const quint32 WordMask=     0x0000ffff;
static const quint64 AllBits64=    0xffffffffffffffff;
static const quint64 MaximumQuint64=0xffffffffffffffff;
static const quint64 HighWord32Mask=   0xffffffff00000000;
static const quint64 Word16Mask=   0x000000000000ffff;
static const quint64 Word32Mask=   0x00000000ffffffff;
static const quint64 Word48Mask=   0x0000ffffffffffff;
static const quint64 Word64HighByte=   0xff00000000000000;
static const quint64 Word64HighNibble= 0xf000000000000000;
static const quint64 Word64HighBit=    0x8000000000000000;
static const int     ByteHighBit= 0x80;  // Must use parens in comparisons, (c&ByteHighBit)==0
static const int     BitsPerByte= 8;
static const int     ByteShift= 8;
static const int     HexShift= 4;       //!< 4 bits per hex digit
static const int     BytesPerInt= 4;
static const int     BytesPerInt64= 8;
static const int     HexDigitsPerInt= 8;
static const int     HexDigitsPerWord64= 16;
static const int     HighHex32Shift= 32-HexShift;
static const int     HighByte32Shift= 32-ByteShift;
static const int     HundredPercentPoints= 10000;
static const int     KByteShift= 10;     //!< Convert bytes to KBytes
static const int     Word16Shift= 16;
static const int     Word24Shift= 24;
static const int     Word32Shift= 32;
static const int     W32= Word32Shift;
static const bool    SelectAll= true;
static const quint64 MaximumThidId64= (HighWord32Mask+Word32Mask);


//!\see ThFindDialog
enum ThFindFlag : ThFlagsInt {
    // Same as QTextDocument::FindFlags
    Backward=           0x00000001,     //!< Find Backwards, not preserved in ThidGuiList::findHistoryList
    CaseSensitive=      0x00000002,     //!< Case-sensitive find
    WholeWord=          0x00000004,     //!< Whole-word find
    // Thesa only
    RegularExpression=  0x00000100,     //!< Regular-expression find
    Unused=             0xfffffef8,     //!< Unused flags
};
Q_DECLARE_FLAGS(ThFindFlags, ThFindFlag)

//! ThHowOwned defines the namespace for tuples
//! Add new ThHowOwned to ThDiskBlock::how_owned_block_type
enum ThHowOwned : ThFlagsInt {
    HowDataMask=    0xFFFFFF00, //!< Mask for data corresponding to HowOwedCode
    HowDataShift=   8,          //!< Shift for data
    HowRangeMask=   0xFFF00000, //!< Mask for range (first) corresponding to HowOwedCode
    HowEndRangeShift= 12,          //!< Shift for end range (ThDiskBlock::howRange)
    HowCodeMask=    0x000000FF, //!< Mask for HowCode
    NoHow=          0x00,       //!< ThHowOwned is undefined
    HowInvalid=     0x10000000, //!< disk_block_id is invalid, never on disk
    HowMatchAllButItem= 0x20000000, //!< Matches anything but HowThidId
    HowAlsoFree=    0x30000000, //!< Block also on ThDiskFreePool, for ThRepairDisk
    //! See ThDiskBlock::how_owned_block_type \todo Review assignment of ThHowOwned codes
    HowThidId=      0x01,       //!< ThidId id_owner with ThidData etc [Thesa's howitem]
    HowThidAlias=   0x02,       //!< Alias ThidId id_owner with a topThidId [Thesa's howitem]
    HowThidTitle=   0x03,       //!< ThidTitle for a ThidId and ThLocaleId, an addition to its label
    HowBootBlock=   0x04,       //!< Boot block at start of DiskBlockPool
    HowBootInformation=  0x05,  //!< Boot Tuples for Thesa: ThDiskFreePool, etc.
    HowItemTable=   0x06,       //!<
    HowItemCache=   0x07,       //!<
    HowXrefBlock=   0x08,       //!<
    HowXrefTable=   0x09,       //!<
    HowThidBucket=  0x0a,       //!< ItemBucketNumber with .data(), HowDataMask is firstHash/lastHash (12 bits apiece)
    HowFreeBlock=   0x0b,       //!< Newly created free block, or erased ThDiskBlockId.  BlockDeleted set
    HowRevisionGroup= 0x0c,     //!< ThRevisionId for a group Thid \todo Combine with HowRevisionThid?
    HowRevisionThid= 0x0d,      //!< ThRevisionId for a Thid
    //!< BlockHeld and BlockReadonly set
    HowDeletedItem= 0x0e,       //!< The item was deleted \todo Not used.  hwdeleted indicated delete from ifowner
    HowMaxSpecial=  0x0e,       // max is 0x1f
    HowOwned=       0x60,       //!< owned by a ThThid
    HowText=        HowOwned+0, //!< 96 Text block (tgtext)
    HowCode=        HowOwned+1, //!< Code block (tgcode)
    HowData=        HowOwned+2, //!< Data block (tgdata1,tgdata2)
    HowMaxOwned=    HowData,    //!< Maximum HowOwned
};//ThHowOwned

enum ThidIdProperties : quint8 {  // Limit to 1 byte of flags for embedding elsewhere
    ThidIdIs64=         0x00,     //!< ThidId is a ThidId64
    ThidIdNot32=        0x03,     //!< ThidId is not 32-bits
    ThidIdNotDepot32=   0x01,     //!< depotIdentifier has more than 32-bits
    ThidIdNotThid32=    0x02,     //!< thidIdentifier has more than 32-bits
    ThidIdIsMask=       0x0c,     //!< Not a ThidId.
     ThidIdIsRevision=   0x04,     //!< A RevisionId.  Maybe qualified by time or location
     ThidIdIsLocation=   0x08,     //!< A LocationId.  Maybe qualified by time.  ThidIdNotDepot32/Thid32 applies to locationId
     ThidIdIsTime=       0x0c,     //!< A ThUtcTime.   No qualifications.
    ThidIdIsUnused=     0x10, // available bit
    ThidIdIsQualified=  0xe0,     //!< ThidId is qualified by location, revision, and/or time
    ThidIdHasRevision=  0x20,     //!< ThidId qualified by revision
    ThidIdHasLocation=  0x40,     //!< ThidId or RevisionId qualified by location
    ThidIdHasTime=      0x80,     //!< ThidId, RevisionId, or LocationId qualified by time
    ThidIdNoProperties= 0xff,     //!< thidIdProperties() not set.  NoThid is ThidIdIs64.
};//ThidIdProperties

//!\see ThXref and ThXrefResults and ThXrefSignatureMatch::search_options
enum ThXrefSearchFlag : ThFlagsInt {
    XrefSearchAll=      0x0ff00000, //!< 'a' Search all thid types and locations
    XrefSearchTypeMask= 0x00f00000, //!< Search types, default is all
    XrefSearchTopics=   0x00100000, //!< 'T' Search topic thids
    XrefSearchQuotes=   0x00200000, //!< 'Q' Search quote thids
    XrefSearchCompiled= 0x00400000, //!< 'C' Search compiled thids
    XrefSearchOther=    0x00800000, //!< 'O' Search other thids
    XrefSearchLocationMask= 0x0f000000, //!< Search locations, default is all
    XrefSearchLabels=   0x01000000, //!< 'l' Search labels
    XrefSearchText=     0x02000000, //!< 't' Search text
    XrefSearchCode=     0x04000000, //!< 'c' Search code
    XrefSearchStrings=  0x08000000, //!< 's' Search strings in code
    XrefSortSubgroupShift=       4, //!< 'H' -- How to subgroup results (mask 0x000000f0)
    XrefSortShift=               8, //!< 'S' -- How to sort results (mask 0x00000f00)
    XrefSortGroupMask=  0x0000000f, //!< 'G' -- How to group results -- room for seven more rules
    XrefSortNone=             0x00, //!< 'G-' Do not group, sort by preference with Where and Type
    XrefSortAge=              0x01, //!< 'Ga' Group by Age, sort by modify date descending (ThidId)
    XrefSortDepot=            0x02, //!< 'Gd' Group by Depot, sort by Title (ThidId)
    XrefSortLabel=            0x03, //!< 'Sl' Sort by label (ThidId).  Group and Subgroup not supported.
    XrefSortPreference=       0x04, //!< 'Sp' Sort by preference descending as defined by ThidSummary  (ThidId)
    XrefSortSignature=        0x05, //!< 'Gx' Group by xref signature, sort by signature+type+where
    XrefSortSize=             0x06, //!< 'Gs' Group and sort by size descending
    XrefSortType=             0x07, //!< 'Gt' Group by signature type (topic, quote, compiled, other)
    XrefSortWhere=            0x08, //!< 'Gw' Group by signature where (title, text, code, string)
    XrefSortMaximum=          0x08, //!< Maximum SortGroup
    XrefSearchDefault=XrefSearchTopics+XrefSearchLabels+XrefSortNone+(XrefSortAge<<XrefSortShift), //!< By default, append ThDiskBlockId and ThidIds, expanding the first 50 to titles
    XrefShowFilter=     0x00001000, //!< 'F' Show filtered results
    XrefShowNotFilter=  0x00002000, //!< 'f' Show !filtered results
    XrefShowData=       0x00004000, //!< 'D' Show data matches
    XrefShowNotData=    0x00008000, //!< 'd' Show !data matches
    XrefShowUnknownData=0x00010000, //!< 'U' Show unknown data (not searched yet)
    XrefShowIdentifiers=0x00020000, //!< 'I' Show ThDiskBlockId and ThidId instead of title
    XrefShowCounts=     0x00040000, //!< 'C' Show signature counts w/o xrefid (ThXrefResults::groupByCounts)
    XrefShowMatchedData=0x00080000, //!< 'M' Show matched data
    XrefShowReverseSort=0x10000000, //!< 'R' Show reverse sort
    XrefShowHeader=     0x40000000, //!< 'h' Show headers
    XrefOrderAgeInfo=   0x20000000, //!< 'A' Order by age from ThidInfo (slower) \todo Eventually will be automatic
    XrefSortUnused=     0x80000000, //!< Unused flags -- 0x8... cannot be used for a QButtonGroup id
    XrefMatchFlags=     0x0000ffff, //!< Flags used only for ThXrefSignatureMatch
    XrefMatchOr=        0x00000001, //!< Set if 'or/+'.  Results are newly found blockids
    XrefMatchNot=       0x00000002, //!< Set if 'not/!'  Results are blockids to cancel
    XrefMatchKeep=      0x00000004, //!< Set if 'keep/++'
                                    //!< Otherwise 'and'. Results are seen blockids
    XrefMatchSourceMask=    0x0000e000, //!< Restrict searches to a specified source, otherwise 0 (Not used yet)
    XrefMatchSourceXref=    0x00002000, //!< Restrict searches to ThXref
    XrefMatchSourceRecent=  0x00004000, //!< Restrict searches to recent ThXrefBlocks
    XrefMatchSourceArchive= 0x00006000, //!< Restrict searches to archive ThXrefBlocks
    XrefMatchSourceSignature= 0x00008000, //!< Restrict searches to signature archive ThXrefBlocks
                                        //!<\todo Restrict searches to xref/recent/archive or old ThXrefBlocks within a date range
};
Q_DECLARE_FLAGS(ThXrefSearchFlags, ThXrefSearchFlag)

}//namespace Thesa

#endif tnamespace_H
