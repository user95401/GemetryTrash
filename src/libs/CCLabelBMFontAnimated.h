//
//  CCLabelAnimated.h
//  CCLabelAnimated
//
//  Created by Steven Barnegren on 23/03/2015.
//
//

/*
 Copyright (c) 2015 Steve Barnegren
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __CCLabelAnimated__CCLabelAnimated__
#define __CCLabelAnimated__CCLabelAnimated__

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class CCLambda : public CCActionInstant {
public:
    std::function<void()> m_callback;
    CCLambda() {};
    virtual ~CCLambda() {};
    virtual void update(float time) {
        m_callback();
    };
    static CCLambda* create(std::function<void()>&& callback) {
        auto ret = new (std::nothrow) CCLambda();
        if (ret) {
            ret->m_callback = std::forward<std::remove_reference_t<decltype(callback)>>(callback);
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    };
};

class CCLabelBMFontAnimated : public CCLabelBMFont {
public:

    // ONLY USE THIS FUNCTION FOR CREATION
    static CCLabelBMFontAnimated* createWithBMFont(const std::string& str, const std::string& fntFile, float width = kCCLabelAutomaticWidth, CCTextAlignment alignment = kCCTextAlignmentLeft, CCPoint imageOffset = CCPointZero)
    {
        auto ret = new CCLabelBMFontAnimated();

        if (ret && ret->initWithString(str.c_str(), fntFile.c_str(), width, alignment, imageOffset))
        {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
    //static CCLabelBMFontAnimated* createWithTTF(const std::string& text, const std::string& fontFile, float fontSize, const CCSize& dimensions = CCPointZero, TextHAlignment hAlignment /* = TextHAlignment::LEFT */, TextVAlignment vAlignment /* = TextVAlignment::TOP */);
    
    //FUNCTIONS TO SET BASIC CHARACTER SPRITE PROPERTIES AT INDEX
    auto getLetter(int index = 0) {
        return getChild<CCSprite>(this, index);
    }
    void setCharScale(int index, float s) {

        if (index >= numLetters()) {
            log::error("{}", "CCLabelBMFontAnimated - Could not set character sprite scale, index out of bounds");
            return;
        }

        cocos2d::CCSprite* charSprite = getLetter(index);
        charSprite->setScale(s);
    };
    void setCharOpacity(int index, float o) {

        if (index >= numLetters()) {
            log::error("{}", "CCLabelBMFontAnimated - Could not set character sprite opacity, index out of bounds");
            return;
        }

        GLubyte opacity = o;
        cocos2d::CCSprite* charSprite = getLetter(index);
        charSprite->setOpacity(opacity);
    };
    void setCharRotation(int index, float r) {

        if (index >= numLetters()) {
            log::error("{}", "CCLabelBMFontAnimated - Could not set character sprite rotation, index out of bounds");
            return;
        }

        cocos2d::CCSprite* charSprite = getLetter(index);
        charSprite->setRotation(r);
    };
    
    //FUNCTIONS TO SET BASIC PROPERTIES OF ALL CHARACTER SPRITES
    void setAllCharsScale(float s) {

        const int numChars = numLetters();

        for (int i = 0; i < numChars; i++) {
            cocos2d::CCSprite* charSprite = getLetter(i);
            charSprite->setScale(s);
        }
    };
    void setAllCharsOpacity(float o) {

        GLubyte opacity = o;

        const int numChars = numLetters();

        for (int i = 0; i < numChars; i++) {
            cocos2d::CCSprite* charSprite = getLetter(i);
            charSprite->setOpacity(opacity);
        }
    };
    void setAllCharsRotation(float r) {

        const int numChars = numLetters();

        for (int i = 0; i < numChars; i++) {
            cocos2d::CCSprite* charSprite = getLetter(i);
            charSprite->setRotation(r);
        }
    };
    void offsetAllCharsPositionBy(CCPoint offset) {

        const int numChars = numLetters();

        for (int i = 0; i < numChars; i++) {
            cocos2d::CCSprite* charSprite = getLetter(i);
            charSprite->setPosition(cocos2d::CCPoint(charSprite->getPosition().x + offset.x, charSprite->getPosition().y + offset.y));
        }
    };
    int numLetters() {

        int index = 0;

        for (; ; ) {

            if (getLetter(index) == nullptr) {
                return index;
            }
            index++;
        }

        return index;
    };
    
    //FUNCTIONS TO RUN CUSTOM ACTIONS ON CHARATER SPRITES
    void runActionOnSpriteAtIndex(int index, CCFiniteTimeAction* action) {

        if (index >= numLetters() || index < 0) {
            log::error("{}", "CCLabelBMFontAnimated::runActionOnSpriteAtIndex - index out of bounds");
            return;
        }

        CCSprite* charSprite = getLetter(index);
        charSprite->runAction(action);

    };
    
    void runActionOnAllSprites(CCAction* action) {

        runActionOnAllSprites(action, false, nullptr);

    };
    void runActionOnAllSprites(CCAction* action, bool removeOnCompletion) {

        runActionOnAllSprites(action, removeOnCompletion, nullptr);

    };
    void runActionOnAllSprites(CCAction* action, bool removeOnCompletion, CCCallFunc *callFuncOnCompletion) {

        const int numChars = numLetters();

        for (int i = 0; i < numChars; i++) {
            cocos2d::CCAction* actionCopy = (CCAction*)action->copy();
            cocos2d::CCSprite* charSprite = getLetter(i);

            if (i == numChars - 1) { //if is the last character, run the call func actions

                CCArray *actionsArray = CCArray::create();
                //CCArray *actionsArray = CCArray::create();

                //actionsArray->addObject((CCFiniteTimeAction*)actionCopy);

                actionsArray->addObject(actionCopy);

                if (callFuncOnCompletion != nullptr) {
                    //actionsArray->addObject((CCFiniteTimeAction*)callFuncOnCompletion);
                    actionsArray->addObject(callFuncOnCompletion);
                }
                if (removeOnCompletion) {
                    actionsArray->addObject(CCCallFunc::create(this, callfunc_selector(CCLabelBMFontAnimated::removeFromParent)));
                }
                CCSequence* actionsSequence = CCSequence::create(actionsArray);
                charSprite->runAction(actionsSequence);
            }
            else { //if is not the last character, just run the action
                charSprite->runAction(actionCopy);
            }
        }
    };
    void stopActionsOnAllSprites() {

        const int numChars = numLetters();

        for (int i = 0; i < numChars; i++) {
            CCSprite* charSprite = getLetter(i);
            charSprite->stopAllActions();
        }
    };
    
    //for the 'run actions sequentially' functions, duration refers to the total time to complete actions on all letters, minus the duration of the action itself
    void runActionOnAllSpritesSequentially(CCFiniteTimeAction* action, float duration, bool removeOnCompletion, CCCallFunc *callFuncOnCompletion) {

        const int numChars = numLetters();

        if (numChars < 2) {
            log::error("{}","CCLabelBMFontAnimated - runActionOnAllSpritesSequentially() requires at least 2 children to operate");
            return;
        }

        for (int i = 0; i < numChars; i++) {

            CCDelayTime* delay = CCDelayTime::create((duration / (numChars - 1)) * i);
            CCAction* actionCopy = (CCAction*)action->copy();
            CCSequence* delayAndAction = CCSequence::create(delay, actionCopy, NULL);
            CCSprite* charSprite = getLetter(i);

            if (i == numChars - 1) { //if is the last character, run the call func actions

                CCArray *actionsArray = CCArray::create();
                actionsArray->addObject(delayAndAction);

                if (callFuncOnCompletion != nullptr) {
                    actionsArray->addObject(callFuncOnCompletion);
                }
                if (removeOnCompletion) {
                    actionsArray->addObject(CCCallFunc::create(this, callfunc_selector(CCLabelBMFontAnimated::removeFromParent)));
                }
                CCSequence* actionsSequence = CCSequence::create(actionsArray);
                charSprite->runAction(actionsSequence);
            }
            else { //if is not the last character, just run the action
                charSprite->runAction(delayAndAction);
            }

        }
    };
    void runActionOnAllSpritesSequentially(CCFiniteTimeAction* action, float duration, bool removeOnCompletion) {

        runActionOnAllSpritesSequentially(action, duration, removeOnCompletion, nullptr);

    };
    void runActionOnAllSpritesSequentially(CCFiniteTimeAction* action, float duration) {

        runActionOnAllSpritesSequentially(action, duration, false, nullptr);

    };
    
    void runActionOnAllSpritesSequentiallyReverse(CCFiniteTimeAction* action, float duration, bool removeOnCompletion, CCCallFunc *callFuncOnCompletion) {

        const int numChars = numLetters();

        if (numChars < 2) {
            log::error("{}","CCLabelBMFontAnimated - runActionOnAllSpritesSequentiallyReverse() requires at least 2 children to operate");
            return;
        }

        for (int i = 0; i < numChars; i++) {

            CCDelayTime* delay = CCDelayTime::create((duration / (numChars - 1)) * ((numChars - 1) - i));
            CCAction* actionCopy = (CCAction*)action->copy();
            CCSequence* delayAndAction = CCSequence::create(delay, actionCopy, NULL);
            CCSprite* charSprite = getLetter(i);

            if (i == 0) { //if is the first character, run the call func actions

                CCArray *actionsArray = CCArray::create();
                actionsArray->addObject(delayAndAction);

                if (callFuncOnCompletion != nullptr) {
                    actionsArray->addObject(callFuncOnCompletion);
                }
                if (removeOnCompletion) {
                    actionsArray->addObject(CCCallFunc::create(this, callfunc_selector(CCLabelBMFontAnimated::removeFromParent)));
                }
                CCSequence* actionsSequence = CCSequence::create(actionsArray);
                charSprite->runAction(actionsSequence);
            }
            else { //if is not the first character, just run the action
                charSprite->runAction(delayAndAction);
            }
        }

    };
    void runActionOnAllSpritesSequentiallyReverse(CCFiniteTimeAction* action, float duration, bool removeOnCompletion) {

        runActionOnAllSpritesSequentiallyReverse(action, duration, removeOnCompletion, nullptr);

    };
    void runActionOnAllSpritesSequentiallyReverse(CCFiniteTimeAction* action, float duration) {

        runActionOnAllSpritesSequentiallyReverse(action, duration, false, nullptr);

    };
    
    //ANIMATIONS

    //fly ins
    void animateInFlyInFromLeft(float duration) {

        CCSize visibleSize = CCDirector::get()->getVisibleSize();
        float rescaleFactor = 1 / getScale(); //if the label has been scaled down, all the action coordinates will be too small, rescale factor scales them up
        float offsetX = visibleSize.width * rescaleFactor;
        offsetAllCharsPositionBy(CCPoint(-offsetX, 0));

        CCMoveBy* flyIn = CCMoveBy::create(1, CCPoint(offsetX, 0));
        CCEaseExponentialOut* flyInEase = CCEaseExponentialOut::create(flyIn);

        runActionOnAllSpritesSequentially(flyInEase, duration);


    };
    void animateInFlyInFromRight(float duration) {

        CCSize visibleSize = CCDirector::get()->getVisibleSize();
        float rescaleFactor = 1 / getScale(); //if the label has been scaled down, all the action coordinates will be too small, rescale factor scales them up
        float offsetX = visibleSize.width * rescaleFactor;
        offsetAllCharsPositionBy(CCPoint(offsetX, 0));

        CCMoveBy* flyIn = CCMoveBy::create(1, CCPoint(-offsetX, 0));
        CCEaseExponentialOut* flyInEase = CCEaseExponentialOut::create(flyIn);

        runActionOnAllSpritesSequentiallyReverse(flyInEase, duration);

    };
    void animateInFlyInFromTop(float duration) {

        CCSize visibleSize = CCDirector::get()->getVisibleSize();
        float rescaleFactor = 1 / getScale(); //if the label has been scaled down, all the action coordinates will be too small, rescale factor scales them up
        float offsetY = visibleSize.height * rescaleFactor;
        offsetAllCharsPositionBy(CCPoint(0, offsetY));

        CCMoveBy* flyIn = CCMoveBy::create(1, CCPoint(0, -offsetY));
        CCEaseExponentialOut* flyInEase = CCEaseExponentialOut::create(flyIn);

        runActionOnAllSpritesSequentially(flyInEase, duration);

    };
    void animateInFlyInFromBottom(float duration) {

        CCSize visibleSize = CCDirector::get()->getVisibleSize();
        float rescaleFactor = 1 / getScale(); //if the label has been scaled down, all the action coordinates will be too small, rescale factor scales them up
        float offsetY = visibleSize.height * rescaleFactor;
        offsetAllCharsPositionBy(CCPoint(0, -offsetY));

        CCMoveBy* flyIn = CCMoveBy::create(1, CCPoint(0, offsetY));
        CCEaseExponentialOut* flyInEase = CCEaseExponentialOut::create(flyIn);

        runActionOnAllSpritesSequentially(flyInEase, duration);

    };
    
    //misc animate ins
    void animateInTypewriter(float duration) {

        //set all the characters scale to zero
        setAllCharsScale(0);

        CCScaleTo* appear = CCScaleTo::create(0, 1);

        runActionOnAllSpritesSequentially(appear, duration);

    };
    void animateInDropFromTop(float duration) {

        CCSize visibleSize = CCDirector::get()->getVisibleSize();
        float rescaleFactor = 1 / getScale(); //if the label has been scaled down, all the action coordinates will be too small, rescale factor scales them up
        float offsetY = visibleSize.height * rescaleFactor;
        offsetAllCharsPositionBy(CCPoint(0, offsetY));

        CCMoveBy* flyIn = CCMoveBy::create(1, CCPoint(0, -offsetY));
        CCEaseBounceOut* flyInEase = CCEaseBounceOut::create(flyIn);

        runActionOnAllSpritesSequentially(flyInEase, duration);

    };
    void animateInSwell(float duration) {

        setAllCharsScale(0);

        CCScaleTo* scaleUp = CCScaleTo::create(0.2, 1.5);
        CCScaleTo* scaleDown = CCScaleTo::create(0.2, 1);
        CCSequence* scaleSeq = CCSequence::create(scaleUp, scaleDown, NULL);

        runActionOnAllSpritesSequentially(scaleSeq, duration);

    };
    void animateInRevealFromLeft(float duration) {

        //set all chars opacity to zero, apart from first
        setAllCharsOpacity(0);
        CCSprite* firstChar = getLetter(0);
        firstChar->setOpacity(255);
        //make sure the first character has higher z order than the rest, reset after the animation
        CCDelayTime* delay = CCDelayTime::create(duration);
        CCLambda* resetZ = CCLambda::create(
            [this, firstChar]() {
                //log::debug("{}", __FUNCTION__);
                reorderChild(firstChar, firstChar->getZOrder());
            }
        );

        CCSequence* resetZAfterAnimation = CCSequence::create(delay, resetZ, NULL);
        this->reorderChild(firstChar, firstChar->getZOrder() + 10);
        firstChar->runAction(resetZAfterAnimation);

        //reveal each char from the behind the first
        for (int i = 1; i < numLetters(); i++) {

            CCSprite* charSprite = getLetter(i);

            CCMoveTo* move = CCMoveTo::create(duration, CCPoint(charSprite->getPosition()));
            CCEaseExponentialOut* moveEase = CCEaseExponentialOut::create(move);
            CCFadeIn* fadeIn = CCFadeIn::create(duration);
            CCEaseExponentialOut* fadeEase = CCEaseExponentialOut::create(fadeIn);
            CCSpawn* moveAndFade = CCSpawn::create(moveEase, fadeEase, NULL);

            charSprite->setPosition(CCPoint(firstChar->getPosition().x, charSprite->getPosition().y));
            charSprite->runAction(moveAndFade);

        }

    };
    void animateInSpin(float duration, int spins) {

        setAllCharsOpacity(0);

        for (int i = 0; i < numLetters(); i++) {

            CCSprite* charSprite = getLetter(i);

            CCMoveTo* moveToPosition = CCMoveTo::create(duration, charSprite->getPosition());
            CCEaseExponentialOut* moveToPositionEase = CCEaseExponentialOut::create(moveToPosition);
            float centreX = this->getContentSize().width / 2;
            charSprite->setPosition(CCPoint(centreX, charSprite->getPosition().y));
            charSprite->runAction(moveToPositionEase);

            CCRotateBy* counterRotate = CCRotateBy::create(duration, -360 * spins);
            CCEaseSineOut* counterRotateEase = CCEaseSineOut::create(counterRotate);
            charSprite->runAction(counterRotateEase);

            CCFadeIn* fadeIn = CCFadeIn::create(duration);
            charSprite->runAction(fadeIn);

        }


        //spin the label
        CCRotateBy* spin = CCRotateBy::create(duration, 360 * spins);
        CCEaseSineOut* spinEase = CCEaseSineOut::create(spin);
        this->runAction(spinEase);


    };
    void animateInVortex(float duration, int spins) {

        this->animateInVortex(false, true, duration, spins);

    };
    
    //misc animations
    void animateSwell(float duration) {

        CCScaleTo* scaleUp = CCScaleTo::create(0.2, 1.5);
        CCScaleTo* scaleDown = CCScaleTo::create(0.2, 1);
        CCSequence* scaleSeq = CCSequence::create(scaleUp, scaleDown, NULL);

        runActionOnAllSpritesSequentially(scaleSeq, duration);

    };
    void animateJump(float duration, float height) {

        const int numChars = numLetters();

        for (int i = 0; i < numLetters(); i++) {


            CCSprite* charSprite = getLetter(i);

            CCDelayTime* delay = CCDelayTime::create((duration / (numChars - 1)) * i);
            CCJumpTo* jump = CCJumpTo::create(0.5, charSprite->getPosition(), height, 1);
            CCSequence* delayThenJump = CCSequence::create(delay, jump, NULL);
            charSprite->runAction(delayThenJump);

        }


    };
    void animateStretchElastic(float stretchDuration, float releaseDuration, float stretchAmount) {

        for (int i = 0; i < numLetters(); i++) {

            CCSprite* charSprite = getLetter(i);

            CCMoveTo* stretch = CCMoveTo::create(stretchDuration,
                CCPoint((charSprite->getPosition().x - (getContentSize().width / 4)) * stretchAmount,
                    charSprite->getPosition().y));
            CCMoveTo* release = CCMoveTo::create(releaseDuration, charSprite->getPosition());
            CCEaseElasticOut* releaseElastic = CCEaseElasticOut::create(release);
            CCSequence* animation = CCSequence::create(stretch, releaseElastic, NULL);

            charSprite->runAction(animation);

        }

    };
    void animateRainbow(float duration) {

        const float tintDuration = 0.2;

        CCTintTo* red = CCTintTo::create(tintDuration, 255, 0, 0);
        CCTintTo* orange = CCTintTo::create(tintDuration, 255, 153, 51);
        CCTintTo* yellow = CCTintTo::create(tintDuration, 255, 255, 0);
        CCTintTo* green = CCTintTo::create(tintDuration, 0, 255, 0);
        CCTintTo* blue = CCTintTo::create(tintDuration, 0, 0, 255);
        CCTintTo* purple = CCTintTo::create(tintDuration, 102, 0, 204);
        CCTintTo* pink = CCTintTo::create(tintDuration, 255, 51, 255);
        CCTintTo* white = CCTintTo::create(tintDuration, 255, 255, 255);

        CCSequence* rainbow = CCSequence::create(red, orange, yellow, green, blue, purple, pink, white, NULL);
        runActionOnAllSpritesSequentially(rainbow, duration);
    };
    void flyPastAndRemove() {

        CCSize visibleSize = CCDirector::get()->getVisibleSize();

        float rescaleFactor = 1 / getScale(); //if the label has been scaled down, all the action coordinates will be too small, rescale factor scales them up

        offsetAllCharsPositionBy(CCPoint(-visibleSize.width * rescaleFactor, 0));

        float centrePortion = visibleSize.width * 0.05;
        float centreSlowTime = 0.9;

        CCMoveBy* flyIn = CCMoveBy::create(0.5, CCPoint((visibleSize.width * rescaleFactor) - ((centrePortion * rescaleFactor) / 2), 0));
        CCEaseExponentialInOut* flyInEase = CCEaseExponentialInOut::create(flyIn);

        CCMoveBy* centreSlowMove = CCMoveBy::create(centreSlowTime, CCPoint(centrePortion * rescaleFactor, 0));
        CCScaleTo* scaleUp = CCScaleTo::create(centreSlowTime / 2, 1.5);
        CCScaleTo* scaleDown = CCScaleTo::create(centreSlowTime / 2, 1);
        CCSequence* swell = CCSequence::create(scaleUp, scaleDown, NULL);
        CCSpawn* centreMoveAndSwell = CCSpawn::create(centreSlowMove, swell, NULL);

        CCMoveBy* flyOut = CCMoveBy::create(0.5, CCPoint((visibleSize.width * rescaleFactor) - ((centrePortion * rescaleFactor) / 2), 0));
        CCEaseExponentialInOut* flyOutEase = CCEaseExponentialInOut::create(flyOut);

        CCSequence* flyPast = CCSequence::create(flyInEase, centreMoveAndSwell, flyOutEase, NULL);

        runActionOnAllSpritesSequentiallyReverse(flyPast, 0.7, true, nullptr);

    };
    
    void animateInVortex(bool removeOnCompletion, bool createGhosts, float duration, int spins) {


        //fade in the label
        float fadeDuration = duration * 0.25;
        CCFadeIn* fadeIn = CCFadeIn::create(fadeDuration);
        this->runAction(fadeIn);

        if (createGhosts) {

            int numGhosts = 3;
            float ghostMaxOpacity = 100;

            for (int i = 0; i < numGhosts; i++) {

                CCLabelBMFontAnimated *ghostLabel = CCLabelBMFontAnimated::createWithBMFont(getString(), getFntFile(), getContentSize().width*2, kCCTextAlignmentCenter);
                ghostLabel->setOpacity(ghostMaxOpacity / (i + 1));
                ghostLabel->setPosition(this->getPosition());
                this->getParent()->addChild(ghostLabel);
                ghostLabel->animateInVortex(true, false, duration, spins);

            }

        }

        for (int i = 0; i < numLetters(); i++) {

            //Alter the number of spins on some characters for variation
            int charSpins = spins;
            if (i % 2 == 0) {
                charSpins--;
            }
            else if (i % 3 == 0) {
                charSpins++;
            }

            //randomly vary the speed of letters
            float staggerAmount = (rand() % 10) / 10.0f;
            float letterDuration = duration + staggerAmount;

            float spinDuration = letterDuration / charSpins;

            CCSprite* charSprite = getLetter(i);

            float radius = fabs((this->getContentSize().width / 2) - charSprite->getPosition().x);
            float squareLength = sqrtf((radius * radius) / 2);
            float arcAmount = radius - squareLength;

            float segmentDuration = spinDuration * 0.25;

            CCArray* spinActions = CCArray::create();

            for (int s = 0; s < charSpins; s++) {

                /*
                 Spin in a circular motion. Circle is split into 4 segments.
                 Segment 1 is the NW arc of the circle
                 Segment 2 is the NE arc of the circle
                 Segment 3 is the SW arc of the circle
                 Segment 4 is the SE arc of the circle
                 */

                 //segment 4
                CCMoveBy* segment4Straight = CCMoveBy::create(segmentDuration, CCPoint(-radius, -radius));
                CCMoveBy* segment4PositiveArc = CCMoveBy::create(segmentDuration / 2, CCPoint(arcAmount, -arcAmount));
                CCEaseSineOut* segment4PositiveArcEase = CCEaseSineOut::create(segment4PositiveArc);
                CCMoveBy* segment4NegativeArc = CCMoveBy::create(segmentDuration / 2, CCPoint(-arcAmount, arcAmount));
                CCEaseSineIn* segment4NegativeArcEase = CCEaseSineIn::create(segment4NegativeArc);
                CCSequence* segment4Arc = CCSequence::create(segment4PositiveArcEase, segment4NegativeArcEase, NULL);
                CCSpawn* segment4Action = CCSpawn::create(segment4Straight, segment4Arc, NULL);

                //segment 3
                CCMoveBy* segment3Straight = CCMoveBy::create(segmentDuration, CCPoint(-radius, radius));
                CCMoveBy* segment3PositiveArc = CCMoveBy::create(segmentDuration / 2, CCPoint(-arcAmount, -arcAmount));
                CCEaseSineOut* segment3PositiveArcEase = CCEaseSineOut::create(segment3PositiveArc);
                CCMoveBy* segment3NegativeArc = CCMoveBy::create(segmentDuration / 2, CCPoint(arcAmount, arcAmount));
                CCEaseSineIn* segment3NegativeArcEase = CCEaseSineIn::create(segment3NegativeArc);
                CCSequence* segment3Arc = CCSequence::create(segment3PositiveArcEase, segment3NegativeArcEase, NULL);
                CCSpawn* segment3Action = CCSpawn::create(segment3Straight, segment3Arc, NULL);

                //segment 1
                CCMoveBy* segment1Straight = CCMoveBy::create(segmentDuration, CCPoint(radius, radius));
                CCMoveBy* segment1PositiveArc = CCMoveBy::create(segmentDuration / 2, CCPoint(-arcAmount, arcAmount));
                CCEaseSineOut* segment1PositiveArcEase = CCEaseSineOut::create(segment1PositiveArc);
                CCMoveBy* segment1NegativeArc = CCMoveBy::create(segmentDuration / 2, CCPoint(arcAmount, -arcAmount));
                CCEaseSineIn* segment1NegativeArcEase = CCEaseSineIn::create(segment1NegativeArc);
                CCSequence* segment1Arc = CCSequence::create(segment1PositiveArcEase, segment1NegativeArcEase, NULL);
                CCSpawn* segment1Action = CCSpawn::create(segment1Straight, segment1Arc, NULL);

                //segment 2
                CCMoveBy* segment2Straight = CCMoveBy::create(segmentDuration, CCPoint(radius, -radius));
                CCMoveBy* segment2PositiveArc = CCMoveBy::create(segmentDuration / 2, CCPoint(arcAmount, arcAmount));
                CCEaseSineOut* segment2PositiveArcEase = CCEaseSineOut::create(segment2PositiveArc);
                CCMoveBy* segment2NegativeArc = CCMoveBy::create(segmentDuration / 2, CCPoint(-arcAmount, -arcAmount));
                CCEaseSineIn* segment2NegativeArcEase = CCEaseSineIn::create(segment2NegativeArc);
                CCSequence* segment2Arc = CCSequence::create(segment2PositiveArcEase, segment2NegativeArcEase, NULL);
                CCSpawn* segment2Action = CCSpawn::create(segment2Straight, segment2Arc, NULL);

                CCSequence* spinAction;

                //Depending on if the letter is to the left or right of the centre of the label, the segments will need to be in different orders:

                if ((this->getContentSize().width / 2) < charSprite->getPosition().x) {
                    spinAction = CCSequence::create(segment4Action, segment3Action, segment1Action, segment2Action, NULL);
                }
                else {
                    spinAction = CCSequence::create(segment1Action, segment2Action, segment4Action, segment3Action, NULL);
                }


                spinActions->addObject(spinAction);


            }

            CCSequence* animation = CCSequence::create(spinActions);
            CCEaseSineOut* animationEase = CCEaseSineOut::create(animation);

            charSprite->runAction(animationEase);
        }

        if (removeOnCompletion) {

            CCDelayTime* waitForAnimation = CCDelayTime::create(duration * 3);
            CCCallFunc* remove = CCCallFunc::create(this, callfunc_selector(CCLabelBMFontAnimated::removeFromParent));
            CCSequence* waitThenRemove = CCSequence::create(waitForAnimation, remove, NULL);
            this->runAction(waitThenRemove);
        }

    };
    
};

#endif /* defined(__CCLabelAnimated__CCLabelAnimated__) */
