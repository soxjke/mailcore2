#include "MCIMAPSearchExpression.h"

using namespace mailcore;

void IMAPSearchExpression::init()
{
	mKind = IMAPSearchKindNone;
	mHeader = NULL;
	mValue = NULL;
	mLeftExpression = NULL;
	mRightExpression = NULL;
    mShouldAvoidCharset = false;
}

IMAPSearchExpression::IMAPSearchExpression()
{
    init();
}

IMAPSearchExpression::IMAPSearchExpression(bool shouldAvoidCharset):IMAPSearchExpression()
{
    mShouldAvoidCharset = shouldAvoidCharset;
}

IMAPSearchExpression::IMAPSearchExpression(IMAPSearchExpression * other)
{
    init();
	mKind = other->mKind;
    mLarger = other->mLarger;
    mShouldAvoidCharset = other->mShouldAvoidCharset;
    MC_SAFE_REPLACE_COPY(String, mHeader, other->mHeader);
    MC_SAFE_REPLACE_COPY(String, mValue, other->mValue);
    MC_SAFE_REPLACE_COPY(IMAPSearchExpression, mLeftExpression, other->mLeftExpression);
    MC_SAFE_REPLACE_COPY(IMAPSearchExpression, mRightExpression, other->mRightExpression);
}

IMAPSearchExpression::~IMAPSearchExpression()
{
    MC_SAFE_RELEASE(mHeader);
	MC_SAFE_RELEASE(mValue);
	MC_SAFE_RELEASE(mLeftExpression);
	MC_SAFE_RELEASE(mRightExpression);
}

String * IMAPSearchExpression::description()
{
    switch (mKind) {
        default:
        case IMAPSearchKindNone:
        return String::stringWithUTF8Format("<%s:%p None>", MCUTF8(className()), this);
        case IMAPSearchKindFrom:
        return String::stringWithUTF8Format("<%s:%p From %s>", MCUTF8(className()), this,
            MCUTF8(mValue->description()));
        case IMAPSearchKindRecipient:
        return String::stringWithUTF8Format("<%s:%p Recipient %s>", MCUTF8(className()), this,
            MCUTF8(mValue->description()));
        case IMAPSearchKindSubject:
        return String::stringWithUTF8Format("<%s:%p Subject %s>", MCUTF8(className()), this,
            MCUTF8(mValue->description()));
        case IMAPSearchKindContent:
        return String::stringWithUTF8Format("<%s:%p Content %s>", MCUTF8(className()), this,
            MCUTF8(mValue->description()));
        case IMAPSearchKindHeader:
        return String::stringWithUTF8Format("<%s:%p Header %s %s>", MCUTF8(className()), this,
            MCUTF8(mHeader->description()), MCUTF8(mValue->description()));
        case IMAPSearchKindOr:
        return String::stringWithUTF8Format("<%s:%p Or %s %s>", MCUTF8(className()), this,
            MCUTF8(mLeftExpression->description()), MCUTF8(mRightExpression->description()));
        case IMAPSearchKindAnd:
        return String::stringWithUTF8Format("<%s:%p And %s %s>", MCUTF8(className()), this,
            MCUTF8(mLeftExpression->description()), MCUTF8(mRightExpression->description()));
    }
}

Object * IMAPSearchExpression::copy()
{
    return new IMAPSearchExpression(this);
}

IMAPSearchExpression * IMAPSearchExpression::searchFrom(String * value)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression();
    expr->mKind = IMAPSearchKindFrom;
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchRecipient(String * value)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression();
    expr->mKind = IMAPSearchKindRecipient;
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchSubject(String * value)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression();
    expr->mKind = IMAPSearchKindSubject;
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchContent(String * value)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression();
    expr->mKind = IMAPSearchKindContent;
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchHeader(String * header, String * value)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression();
    expr->mKind = IMAPSearchKindHeader;
    MC_SAFE_REPLACE_COPY(String, expr->mHeader, header);
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchLarger(uint32_t value)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression();
    expr->mKind = IMAPSearchKindLarger;
    expr->mLarger = value;
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchAnd(IMAPSearchExpression * left, IMAPSearchExpression * right)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression();
    expr->mKind = IMAPSearchKindAnd;
    MC_SAFE_REPLACE_RETAIN(IMAPSearchExpression, expr->mLeftExpression, left);
    MC_SAFE_REPLACE_RETAIN(IMAPSearchExpression, expr->mRightExpression, right);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchOr(IMAPSearchExpression * left, IMAPSearchExpression * right)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression();
    expr->mKind = IMAPSearchKindOr;
    MC_SAFE_REPLACE_RETAIN(IMAPSearchExpression, expr->mLeftExpression, left);
    MC_SAFE_REPLACE_RETAIN(IMAPSearchExpression, expr->mRightExpression, right);
    return (IMAPSearchExpression *) expr->autorelease();
}

// charset-controled methods

IMAPSearchExpression * IMAPSearchExpression::searchFrom(String * value, bool shouldAvoidCharset)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression(shouldAvoidCharset);
    expr->mKind = IMAPSearchKindFrom;
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchRecipient(String * value, bool shouldAvoidCharset)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression(shouldAvoidCharset);
    expr->mKind = IMAPSearchKindRecipient;
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchSubject(String * value, bool shouldAvoidCharset)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression(shouldAvoidCharset);
    expr->mKind = IMAPSearchKindSubject;
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchContent(String * value, bool shouldAvoidCharset)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression(shouldAvoidCharset);
    expr->mKind = IMAPSearchKindContent;
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchHeader(String * header, String * value, bool shouldAvoidCharset)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression(shouldAvoidCharset);
    expr->mKind = IMAPSearchKindHeader;
    MC_SAFE_REPLACE_COPY(String, expr->mHeader, header);
    MC_SAFE_REPLACE_COPY(String, expr->mValue, value);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchLarger(uint32_t value, bool shouldAvoidCharset)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression(shouldAvoidCharset);
    expr->mKind = IMAPSearchKindLarger;
    expr->mLarger = value;
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchAnd(IMAPSearchExpression * left, IMAPSearchExpression * right, bool shouldAvoidCharset)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression(shouldAvoidCharset);
    expr->mKind = IMAPSearchKindAnd;
    MC_SAFE_REPLACE_RETAIN(IMAPSearchExpression, expr->mLeftExpression, left);
    MC_SAFE_REPLACE_RETAIN(IMAPSearchExpression, expr->mRightExpression, right);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchExpression * IMAPSearchExpression::searchOr(IMAPSearchExpression * left, IMAPSearchExpression * right, bool shouldAvoidCharset)
{
    IMAPSearchExpression * expr = new IMAPSearchExpression(shouldAvoidCharset);
    expr->mKind = IMAPSearchKindOr;
    MC_SAFE_REPLACE_RETAIN(IMAPSearchExpression, expr->mLeftExpression, left);
    MC_SAFE_REPLACE_RETAIN(IMAPSearchExpression, expr->mRightExpression, right);
    return (IMAPSearchExpression *) expr->autorelease();
}

IMAPSearchKind IMAPSearchExpression::kind()
{
    return mKind;
}

String * IMAPSearchExpression::header()
{
    return mHeader;
}

String * IMAPSearchExpression::value()
{
    return mValue;
}

uint32_t IMAPSearchExpression::larger()
{
    return mLarger;
}

bool IMAPSearchExpression::shouldAvoidCharset()
{
    return mShouldAvoidCharset;
}

IMAPSearchExpression * IMAPSearchExpression::leftExpression()
{
    return mLeftExpression;
}

IMAPSearchExpression * IMAPSearchExpression::rightExpression()
{
    return mRightExpression;
}
