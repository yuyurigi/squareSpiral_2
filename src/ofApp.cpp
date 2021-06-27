#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    //color
    backColor = ofColor::fromHex(0xf2f2f2);
    colors.push_back(ofColor::fromHex(0xFEC5BB));
    colors.push_back(ofColor::fromHex(0xFCD5CE));
    colors.push_back(ofColor::fromHex(0xFAE1DD));
    colors.push_back(ofColor::fromHex(0xf6edeb));
    colors.push_back(ofColor::fromHex(0xe3efed));
    colors.push_back(ofColor::fromHex(0xd4e8d9));
    colors.push_back(ofColor::fromHex(0xe9ecdb));
    colors.push_back(ofColor::fromHex(0xFFE5D9));
    colors.push_back(ofColor::fromHex(0xFFD7BA));
    colors.push_back(ofColor::fromHex(0xFEC89A));
   
    lineWidth = 4; //線の太さ
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fbo2.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F_ARB);
    
    fbo.begin();
    ofBackground(backColor);
    fbo.end();
    
    fbo2.begin();
    ofBackground(255, 255, 255);
    fbo2.end();
    
    count = 5;
    count2 = 1;
    min = 20; // 最大四角形の最小半径
    max = 30; // 最大四角形の最大半径
    minMin = 2; //最小四角形の最小半径
    minMax = 10; //最小四角形の最大半径
    bGuide = false; // 描く予定の大きさの四角形を表示
    bEasing = true; // 四角形の半径の大きさを変えていくのにイージングを使う（使わない場合は均等に半径が変化していく）
    mode = 2;
    colorStep = 0;

}

//--------------------------------------------------------------
void ofApp::update(){
    fbo.begin();
    if(count < 5){
        draw1();
    }
    if (count2 < 1) {
        draw2();
    }
    fbo.end();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255, 255, 255);
    
    ofSetColor(255, 255, 255);
    fbo.draw(0, 0);
    
    //backImg2.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::draw1(){
    if (squareNum != prevSquareNum) { //新しい四角を描き始めるとき
        
        _addRadius = ofRandom(3.0, 4.0); ////線と線の間隔
        
        if(squareNum == 0){
            rotway = calcRotway();
        }
        if(squareNum > 0) {
            prevRotway = rotway;
            rotway = nextRotway;
        }
    }
    
    if (rotway==0) {
        drawSmall();
    } else {
        drawBig();
    }
}
//--------------------------------------------------------------
void ofApp::drawSmall(){
    
    if (squareNum != prevSquareNum) { //新しい四角を描き始めるとき
        bCheck = false;
        if (squareNum != 0) prevMinRadius = minRadius;
        minRadius = ofRandom(minMin, minMax);
        count = 0;
        count3 = 0;
        step = 0;
        prevSquareNum = squareNum;
        
        addRadius = _addRadius;
        easing = ofRandom(0.098, 0.11);
        
        if (squareNum == 0) { //一個目
            direction = 0;
            radius = ofRandom(min, max);
            radiusCopy = radius;
            centerPos = glm::vec2(pos.x+minRadius, pos.y+minRadius);
            a = pos.x+minRadius*2;
            b = pos.y;
            ang = -HALF_PI/2;
        } else { //２個目以降
            if(prevRotway == 0) { //１個前が小さいほうから描いていく四角形だった場合
                switch (direction%4) {
                    case 0: //→
                        centerPos = glm::vec2(pos.x+prevRadius*2+radius, pos.y+minRadius);
                        a = pos.x+prevRadius*2+radius+minRadius;
                        b = pos.y;
                        break;
                    case 1: //↓
                        centerPos = glm::vec2(pos.x-minRadius/2, pos.y+prevRadius*2+radius);
                        a = pos.x;
                        b = pos.y+prevRadius*2+radius+minRadius;
                        break;
                    case 2: //←
                        centerPos = glm::vec2(pos.x-prevRadius*2-radius, pos.y-minRadius);
                        a = pos.x-prevRadius*2-radius-minRadius;
                        b=pos.y;
                        break;
                    case 3: //↑
                        centerPos = glm::vec2(pos.x+minRadius, pos.y-prevRadius*2-radius);
                        a = pos.x;
                        b = pos.y-prevRadius*2-radius-minRadius;
                        break;
                }
            } else if (prevRotway == 1){ //１個前が大きいほうから描いていく四角形だった場合
                switch (direction%4) {
                    case 0: //→
                        centerPos = glm::vec2(pos.x+prevMinRadius+prevRadius+radius, pos.y+minRadius);
                        a = pos.x+prevMinRadius+prevRadius+radius+minRadius;
                        b = pos.y;
                        break;
                    case 1: //↓
                        centerPos = glm::vec2(pos.x-minRadius, pos.y+prevMinRadius+prevRadius+radius);
                        a = pos.x;
                        b = pos.y+prevMinRadius+prevRadius+radius+minRadius;
                        break;
                    case 2: //←
                        centerPos = glm::vec2(pos.x-prevMinRadius-prevRadius-radius, pos.y-minRadius);
                        a = pos.x-prevMinRadius-prevRadius-radius-minRadius;
                        b=pos.y;
                        break;
                    case 3: //↑
                        centerPos = glm::vec2(pos.x+minRadius, pos.y-prevMinRadius-prevRadius-radius);
                        a = pos.x;
                        b = pos.y-prevMinRadius-prevRadius-radius-minRadius;
                        break;
                }
            }
        }
        
        radiusCopy = ofDist(centerPos.x, centerPos.y, centerPos.x+radius, centerPos.y+radius);
        minRadiusCopy = ofDist(centerPos.x, centerPos.y, centerPos.x+minRadius, centerPos.y+minRadius);
        
        if(bGuide) drawOutlineRect(centerPos, radius, ofColor(255, 173, 44));
    } else { //すでに描いてる四角にうずまきを描き足していくとき
        bCheck = true;
        if (bEasing) {
            float minRadiusCopy2;
            if(count<1){
            minRadiusCopy2 = minRadiusCopy*easing;
            }
            minRadiusCopy += minRadiusCopy2;
        } else {
            minRadiusCopy += addRadius;
        }
        a = centerPos.x + (minRadiusCopy*cos(ang));
        b = centerPos.y + (minRadiusCopy*sin(ang));
    }
    
    lastPos = pos;
    pos = glm::vec2(a, b);
    
    //posが画面外のときは四角形の描画を止める
    if ((pos.x<0) || (ofGetWidth()<pos.x) || (pos.y<0) || (ofGetHeight()<pos.y)) {
        count = 5;
    } else {
        // 線を描く
        //ofSetLineWidth(2);
        //ofSetColor(lineColor);
        //ofDrawLine(lastPos, pos);
        
        if (colorStep % 50 == 0) {
            r = (int)ofRandom(colors.size());
        }
        ofSetColor(colors[r]);
        
        roundLine(lastPos, pos, lineWidth);
    }
    
    ang += HALF_PI;
    direction += 1;
    step += 1;
    colorStep += 1;
    
    //今描いてる四角形を終わりにする
    if (bCheck) {
        float d = ofDist(lastPos.x, lastPos.y, pos.x, pos.y);
        ofColor color = readBackground(pos.x, pos.y);
        
        //現在位置(pos)が別の四角形に重なってるとき
        //posの色が白でないとき(fbo2から色をとってる）
        //もしくは、 lastPosとposの距離が最初に設定した四角形の直径を超えてるとき
        float border;
        if(mode == 0 || mode == 2){
            border = radius*2;
        } else if(mode == 3){
            border = radius*2*0.4;
        }
        if (((color.r!=255)&&(color.g!=255)&&(color.b!=255)) || (d>border)){
            count3+=1; //別の四角形と重なっていても１周する
            
            if(count3>4){
                nextRotway = calcRotway();
                
                if(nextRotway==0){
                    smallToSmall();
                } else if (nextRotway == 1){
                    smallToBig();
                }
            }
            
        }
    }
    
}
//--------------------------------------------------------------
void ofApp::smallToSmall(){
    // 次の頂点を計算する
    float prevRadius_copy = radius;
    float radius_copy = ofRandom(min, max);
    glm::vec2 nextPos;
    switch (direction%4) {
        case 0: //→
            nextPos = glm::vec2(pos.x+prevRadius_copy*2+radius_copy, pos.y);
            break;
        case 1: //↓
            nextPos = glm::vec2(pos.x, pos.y+prevRadius_copy*2+radius);
            break;
        case 2: //←
            nextPos = glm::vec2(pos.x-prevRadius_copy*2-radius_copy, pos.y);
            break;
        case 3: //↑
            nextPos = glm::vec2(pos.x, pos.y-prevRadius_copy*2-radius_copy);
            break;
    }
    
    //次の頂点がすでに描いてる四角と重ならなかったら、次の四角形に移る
    //重なっていたら、現在の四角形の描画を続行する
    //(count+=1)する
    //4方向とも新しい四角形が描けるスペースがない場合は四角の描画を止める
    ofColor color = readBackground(nextPos.x, nextPos.y);
    if ((color.r==255)&&(color.g==255)&&(color.b==255)) {
        float d = ofDist(lastPos.x, lastPos.y, pos.x, pos.y);
        if(d>radius*2){
            prevRadius = radius;
        } else {
            prevRadius = d/2;
            
            if(bGuide) drawOutlineRect(centerPos, d/2, ofColor(255, 0, 0));
        }
        radius = radius_copy;
        radiusCopy = radius;
        squareNum += 1;
        
        writeFbo2(centerPos, d/2);
    } else {
        count += 1;
    }
    
}
//--------------------------------------------------------------
void ofApp::smallToBig(){
    // 次の頂点を計算する
    float prevRadius_copy = radius;
    float radius_copy = ofRandom(min, max);
    glm::vec2 nextPos[4];
    switch (direction%4) {
        case 0: //→
            nextPos[0] = glm::vec2(pos.x+prevRadius_copy*2+radius_copy, pos.y+radius_copy); //次の四角形の中心
            nextPos[1] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y-radius_copy); //次の四角形の右上
            nextPos[2] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y+radiusCopy); //次の四角形の右下
            nextPos[3] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y+radiusCopy); //次の四角形の左下
            break;
        case 1: //↓
            nextPos[0] = glm::vec2(pos.x-radius_copy, pos.y+prevRadius_copy*2+radius_copy); //次の四角形の中心
            nextPos[1] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y+radius_copy); //次の四角形の右下
            nextPos[2] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y+radiusCopy); //次の四角形の左下
            nextPos[3] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y-radiusCopy); //次の四角形の左上
            break;
        case 2: //←
            nextPos[0] = glm::vec2(pos.x-prevRadius_copy*2-radius_copy, pos.y-radius_copy); //次の四角形の中心
            nextPos[1] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y+radius_copy); //次の四角形の左下
            nextPos[2] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y-radiusCopy); //次の四角形の左上
            nextPos[3] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y-radiusCopy); //次の四角形の右上
            break;
        case 3: //↑
            nextPos[0] = glm::vec2(pos.x+radius_copy, pos.y-prevRadius_copy*2-radius_copy); //次の四角形の中心
            nextPos[1] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y-radius_copy); //次の四角形の左上
            nextPos[2] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y-radiusCopy); //次の四角形の右上
            nextPos[3] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y+radiusCopy); //次の四角形の右下
            break;
    }
    
    //次の頂点がすでに描いてる四角と重ならなかった場合と、次の次の頂点が画面外でない場合、次の四角形に移る
    //重なっていたら、現在の四角形の描画を続行する
    //(count+=1)する
    //4方向とも新しい四角形が描けるスペースがない場合は四角の描画を止める
    ofColor color[4];
    for (int i = 0; i < 4; i++) {
        color[i] = readBackground(nextPos[i].x, nextPos[i].y);
    }
    if ((color[0].r==255)&&(color[1].r==255)&&(color[2].r==255)&&(color[3].r=255)) {
        if((0<nextPos[2].x)&&(nextPos[2].x<ofGetWidth())&&(0<nextPos[2].y)&&(nextPos[2].y<ofGetHeight())){
            float d = ofDist(lastPos.x, lastPos.y, pos.x, pos.y);
            if(d>radius*2){
                prevRadius = radius;
            } else {
                prevRadius = d/2;
                
                if(bGuide) drawOutlineRect(centerPos, d/2, ofColor(255, 0, 0));
            }
            radius = radius_copy;
            radiusCopy = radius;
            squareNum += 1;
            
            writeFbo2(centerPos, d/2);
        }
    } else {
        count += 1;
    }
    
}
//--------------------------------------------------------------
void ofApp::drawBig(){
    if (squareNum != prevSquareNum) { //新しい四角を描き始めるとき
        bCheck = false;
        if (squareNum != 0) prevMinRadius = minRadius;
        count = 0;
        prevSquareNum = squareNum;
        
        addRadius = _addRadius;
        easing = ofRandom(0.09, 0.1);
        
        minRadius = ofRandom(minMin, minMax);
        
        if (squareNum == 0) { //一番最初
            direction = 0;
            radius = ofRandom(min, max);
            //radiusCopy = radius;
            centerPos = glm::vec2(pos.x+radius, pos.y+radius);
            //次の頂点
            a = pos.x+radius*2;
            b = pos.y;
            ang = -HALF_PI/2;
        } else { //２個目以降
            //direction = direction%4;
            if(prevRotway == 0) { //１個前が小さいほうから描いていく四角形だった場合
                switch (direction%4) {
                    case 0: //→
                        centerPos = glm::vec2(pos.x+prevRadius*2+radius, pos.y+radius);
                        a = pos.x+prevRadius*2+radius*2;
                        b = pos.y;
                        break;
                    case 1: //↓
                        centerPos = glm::vec2(pos.x-radius, pos.y+prevRadius*2+radius);
                        a = pos.x;
                        b = pos.y+prevRadius*2+radius*2;
                        break;
                    case 2: //←
                        centerPos = glm::vec2(pos.x-prevRadius*2-radius, pos.y-radius);
                        a = pos.x-prevRadius*2-radius*2;
                        b=pos.y;
                        break;
                    case 3: //↑
                        centerPos = glm::vec2(pos.x+radius, pos.y-prevRadius*2-radius);
                        a = pos.x;
                        b = pos.y-prevRadius*2-radius*2;
                        break;
                }
            } else if (prevRotway == 1){ //１個前が大きいほうから描いていく四角形だった場合
                switch (direction%4) {
                    case 0: //→
                        centerPos = glm::vec2(pos.x+prevMinRadius+prevRadius+radius, pos.y+radius);
                        a = pos.x+prevMinRadius+prevRadius+radius*2;
                        b = pos.y;
                        break;
                    case 1: //↓
                        centerPos = glm::vec2(pos.x-prevMinRadius-radius, pos.y+prevMinRadius+prevRadius+radius);
                        a = pos.x;
                        b = pos.y+prevMinRadius+prevRadius+radius*2;
                        break;
                    case 2: //←
                        centerPos = glm::vec2(pos.x-prevMinRadius-prevRadius-radius, pos.y-radius);
                        a = pos.x-prevMinRadius-prevRadius-radius*2;
                        b=pos.y;
                        break;
                    case 3: //↑
                        centerPos = glm::vec2(pos.x+radius, pos.y-prevMinRadius-prevRadius-radius);
                        a = pos.x;
                        b = pos.y-prevMinRadius-prevRadius-radius*2;
                        break;
                }
            }
        }
        
        radiusCopy = ofDist(centerPos.x, centerPos.y, centerPos.x+radius, centerPos.y+radius);
        minRadiusCopy = ofDist(centerPos.x, centerPos.y, centerPos.x+minRadius, centerPos.y+minRadius);
        
        if(bGuide) drawOutlineRect(centerPos, radius, ofColor(255, 173, 44));
    } else { //すでに描いてる四角にうずまきを描き足していくとき
        bCheck = true;
        
        if (bEasing) {
            // うずまきの半径を小さくするのにeasingをかける
            radiusCopy -= (radiusCopy) * easing;
        } else {
            // うずまきの半径を均等に小さくする
            radiusCopy = abs(radiusCopy) - addRadius;
        }
        a = centerPos.x + (radiusCopy*cos(ang));
        b = centerPos.y + (radiusCopy*sin(ang));
    }
    
    lastPos = pos;
    pos = glm::vec2(a, b);
    
    //posが画面外のときは四角形の描画を止める
    if ((pos.x<0) || (ofGetWidth()<pos.x) || (pos.y<0) || (ofGetHeight()<pos.y)) {
        count = 5;
    } else {
        // 線を描く
        //ofSetLineWidth(2);
        //ofSetColor(lineColor);
        //ofDrawLine(lastPos, pos);
        if (colorStep % 50 == 0) {
            r = (int)ofRandom(colors.size());
        }
        ofSetColor(colors[r]);
        roundLine(lastPos, pos, lineWidth);
    }
    
    ang += HALF_PI;
    direction += 1;
    colorStep += 1;
    
    //今描いてる四角形を終わりにする
    if (bCheck) {
        float d = ofDist(lastPos.x, lastPos.y, pos.x, pos.y);
        ofColor color = readBackground(pos.x, pos.y);
        
        //lastPosとposの距離が最初に設定した四角形の最小直径より小さいとき
        if (d<minRadius*2){
            
            nextRotway = calcRotway();
            
            if(nextRotway==0){
                bigToSmall();
            } else if (nextRotway == 1){
                bigToBig();
            }
            
        }
    }
}
//--------------------------------------------------------------
void ofApp::bigToSmall(){
    // 次の頂点を計算する
    float prevRadius_copy = radius; //今の四角形の大きさ
    float prevMinRadius_copy = minRadius; //今の四角形の最小半径
    float radius_copy = ofRandom(min, max); //次の四角形の大きさ
    glm::vec2 nextPos;
    switch (direction%4) {
        case 0: //→
            nextPos = glm::vec2(pos.x+prevMinRadius_copy+prevRadius_copy+radius_copy, pos.y);
            break;
        case 1: //↓
            nextPos = glm::vec2(pos.x, pos.y+prevMinRadius_copy+prevRadius_copy+radius_copy);
            break;
        case 2: //←
            nextPos = glm::vec2(pos.x-prevMinRadius_copy-prevRadius_copy-radius_copy, pos.y);
            break;
        case 3: //↑
            nextPos = glm::vec2(pos.x, pos.y-prevMinRadius_copy-prevRadius_copy-radius_copy);
            break;
    }
    
    //次の頂点がすでに描いてる四角と重ならなかった場合、次の四角形に移る
    //重なっていたら、現在の四角形の描画を続行する
    //(count+=1)する
    //4方向とも新しい四角形が描けるスペースがない場合は四角の描画を止める
    ofColor color = readBackground(nextPos.x, nextPos.y);
    if ((color.r==255)&&(color.r==255)&&(color.r==255)) {
            prevRadius = radius;
            radius = radius_copy;
            squareNum += 1;
            
            writeFbo2(centerPos, prevRadius);
    } else {
        count += 1;
    }
}
//--------------------------------------------------------------
void ofApp::bigToBig(){
    // 次の頂点を計算する
    float prevRadius_copy = radius; //今の四角形の大きさ
    float prevMinRadius_copy = minRadius; //今の四角形の最小半径
    float radius_copy = ofRandom(min, max); //次の四角形の大きさ
    glm::vec2 nextPos[4];
    switch (direction%4) {
        case 0: //→
            nextPos[0] = glm::vec2(pos.x+prevMinRadius_copy+prevRadius_copy+radius_copy, pos.y+radius_copy); //次の四角形の中心
            nextPos[1] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y-radius_copy); //次の四角形の右上
            nextPos[2] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y+radius_copy); //次の四角形の右下
            nextPos[3] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y+radius_copy); //次の四角形の左下
            break;
        case 1: //↓
            nextPos[0] = glm::vec2(pos.x-radius_copy, pos.y+prevMinRadius_copy+prevRadius_copy+radius_copy); //次の四角形の中心
            nextPos[1] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y+radius_copy); //次の四角形の右下
            nextPos[2] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y+radius_copy); //次の四角形の左下
            nextPos[3] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y-radius_copy); //次の四角形の左上
            break;
        case 2: //←
            nextPos[0] = glm::vec2(pos.x-prevMinRadius_copy-prevRadius_copy-radius_copy, pos.y-radius_copy); //次の四角形の中心
            nextPos[1] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y+radius_copy); //次の四角形の左下
            nextPos[2] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y-radius_copy); //次の四角形の左上
            nextPos[3] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y-radius_copy); //次の四角形の右上
            break;
        case 3: //↑
            nextPos[0] = glm::vec2(pos.x+radius_copy, pos.y-prevMinRadius_copy-prevRadius_copy-radius_copy); //次の四角形の中心
            nextPos[1] = glm::vec2(nextPos[0].x-radius_copy, nextPos[0].y-radius_copy); //次の四角形の左上
            nextPos[2] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y-radius_copy); //次の四角形の右上
            nextPos[3] = glm::vec2(nextPos[0].x+radius_copy, nextPos[0].y+radius_copy); //次の四角形の右下
            break;
    }
    
    //次の頂点がすでに描いてる四角と重ならなかった場合と、次の次の頂点が画面外でない場合、次の四角形に移る
    //重なっていたら、現在の四角形の描画を続行する
    //(count+=1)する
    //4方向とも新しい四角形が描けるスペースがない場合は四角の描画を止める
    ofColor color[4];
    for (int i = 0; i < 4; i++) {
        color[i] = readBackground(nextPos[i].x, nextPos[i].y);
    }
    if ((color[0].r==255)&&(color[1].r==255)&&(color[2].r==255)&&(color[3].r=255)) {
        if((0<nextPos[2].x)&&(nextPos[2].x<ofGetWidth())&&(0<nextPos[2].y)&&(nextPos[2].y<ofGetHeight())){
            prevRadius = radius;
            radius = radius_copy;
            squareNum += 1;
            
            writeFbo2(centerPos, prevRadius);
        }
    } else {
        count += 1;
    }
}

//--------------------------------------------------------------
void ofApp::draw2(){
    //四角のうずまきを１セットだけ描く
    if (squareNum != prevSquareNum) { //新しい四角を描き始めるとき
        bCheck = false;
        direction = 0;
        step = 0;
        minRadius = ofRandom(minMin, minMax);
        prevSquareNum = squareNum;
        ang = -HALF_PI/2;
        
        addRadius = ofRandom(1.5, 3.0);
        easing = ofRandom(0.098, 0.11);
        //float random = ofRandom(1);
        float random = 0;
        if (random < 0.6){
            bEasing = true;
        } else {
            bEasing = false;
        }
        
        if (squareNum == 0) { //一番最初
            radius = ofRandom(min, max);
            centerPos = glm::vec2(pos.x+minRadius, pos.y+minRadius);
            a = pos.x+minRadius*2;
            b = pos.y;
        }
        
        minRadiusCopy = ofDist(centerPos.x, centerPos.y, centerPos.x+minRadius, centerPos.y+minRadius);
        
        if(bGuide) drawOutlineRect(centerPos, radius, ofColor(255, 173, 44));
    } else { //すでに描いてる四角にうずまきを描き足していくとき
        bCheck = true;
        if (bEasing) {
            minRadiusCopy += minRadiusCopy * easing;
        } else {
            minRadiusCopy += addRadius;
        }
        a = centerPos.x + (minRadiusCopy*cos(ang));
        b = centerPos.y + (minRadiusCopy*sin(ang));
    }
    
    lastPos = pos;
    pos = glm::vec2(a, b);
    
    //posが画面外のときは四角形を描くのをやめる
    if ((pos.x<0) || (ofGetWidth()<pos.x) || (pos.y<0) || (ofGetHeight()<pos.y)) {
        count = 1;
    }
    
    //線を描く
    //ofSetLineWidth(2);
    //ofSetColor(lineColor);
    //ofDrawLine(lastPos, pos);
    if (colorStep % 50 == 0) {
        r = (int)ofRandom(colors.size());
    }
    ofSetColor(colors[r]);
    roundLine(lastPos, pos, lineWidth);
    
    ang += HALF_PI;
    direction += 1;
    step += 1;
    colorStep += 1;
    
    //四角形を描くのをやめるタイミング
    if (bCheck) {
        float d = ofDist(lastPos.x, lastPos.y, pos.x, pos.y);
        
        //lastPosとposの長さが最初に設定した四角形の直径を超えてるとき
        if (d>radius*2){
            count2 += 1;
            
            writeFbo2(centerPos, radius);
        }
    }
}
//--------------------------------------------------------------
int ofApp::calcRotway(){
    float random;
    if(mode == 0) random = 0;
    if(mode == 1) random = 1;
    if(mode == 2 || mode == 3) {
        random = ofRandom(1);
    }
    int rw;
    if (random<0.5) {
        rw = 0;
    } else {
        rw = 1;
    }
    return rw;
}
//--------------------------------------------------------------
void ofApp::writeFbo2(glm::vec2 centerP, float rad){
    fbo2.begin();
    ofSetColor(0);
    ofFill();
    ofPushMatrix();
    ofTranslate(centerP);
    ofDrawRectangle(-rad, -rad, rad*2, rad*2);
    ofPopMatrix();
    fbo2.end();
}
//--------------------------------------------------------------
void ofApp::drawOutlineRect(glm::vec2 centerP, float rad, ofColor col){
    ofSetColor(col);
    ofNoFill();
    ofPushMatrix();
    ofTranslate(centerP);
    ofDrawRectangle(-rad, -rad, rad*2, rad*2);
    ofPopMatrix();
}
//--------------------------------------------------------------
ofColor ofApp::readBackground(float x, float y){
    ofPixels colorPixels;
    fbo2.readToPixels(colorPixels);
    ofColor c;
    if(0<x && x<ofGetWidth() && 0<y && 0<ofGetHeight()){
        c = colorPixels.getColor(x, y);
    } else {
        c = ofColor(0, 0, 0);
    }
    return c;
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'S' || key == 's') { //スクリーンショット
            ofImage myImage;
            myImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            myImage.save(ofGetTimestampString("%Y%m%d%H%M%S")+"##.jpg");
    }
    
    if (key =='C' || key == 'c') { //画面をクリア
        //描いてる四角形を止める
        count = 5;
        count2 = 1;
        colorStep = 0;
        
        fbo.begin();
        //backImg.draw(0, 0);
        ofSetColor(backColor);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        fbo.end();
        
        fbo2.begin();
        //backImg3.draw(0, 0);
        ofSetColor(255, 255, 255);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        fbo2.end();
    }
    
    if (key == '0') mode = 0; // 小さい方から描くうずまきだけ
    if (key == '1') mode = 1; // 大きい方から描くうずまきだけ
    if (key == '2') mode = 2; // 両方混合（デフォルト）
    if (key == '3') mode = 3; // 両方混合（四角と四角に間隔がある）
    if (key =='E' || key == 'e') {
        bEasing = !bEasing;
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // 四角を描いてないところを押したら
    ofColor color = readBackground(x, y);
    if ((color.r==255)&&(color.g==255)&&(color.b==255)) {
        
        if ((count < 5) || (count2 < 1)) {
            float d = ofDist(lastPos.x, lastPos.y, pos.x, pos.y);
            writeFbo2(centerPos, d/2);
            if(bGuide) drawOutlineRect(centerPos, d/2, ofColor(255, 0, 0));
        }
        
        //button (left 0, center 1, right 2)
        if (button == 0) {
            //左ボタン押したら
            pos = glm::vec2(x, y);
            squareNum = 0;
            prevSquareNum = -1;
            count = 0;
            count2 = 1;
        }
    }
    
    //右ボタン押したら
    //四角のうずまきをひとつだけ描く
    //（場所はどこでも）
    if (button == 2){
        pos = glm::vec2(x, y);
        squareNum = 0;
        prevSquareNum = -1;
        count = 5;
        count2 = 0;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
//-----------------------roundLine------------------------------
//--------------------------------------------------------------

void ofApp::roundLine(glm::vec2 p0, glm::vec2 p1, float _lineWidth){
    drawCap_round_first(p0, p1, _lineWidth);
    drawLine(p0, p1, _lineWidth);
    drawCap_round_last(p0, p1, _lineWidth);
}

//--------------------------------------------------------------
void ofApp::drawLine(glm::vec2 p0, glm::vec2 p1, float _lineWidth){
    glm::vec2 base;
    glm::vec2 T[4];
    float half_lineWidth = _lineWidth/2;
    //最初のライン
    base = p0-p1;
    base = glm::normalize(base);
    base *= half_lineWidth;
    T[0] = p0 + perpen(base);
    T[1] = p0 + inverse(perpen(base));
    T[2] = p1+ perpen(base);
    T[3] = p1 + inverse(perpen(base));
        
    ofMesh mesh1;
    mesh1.addVertex(ofPoint(T[0].x, T[0].y, 0));
    mesh1.addVertex(ofPoint(T[1].x, T[1].y, 0));
    mesh1.addVertex(ofPoint(T[2].x, T[2].y, 0));
    mesh1.addVertex(ofPoint(T[3].x, T[3].y, 0));
    mesh1.addIndex(0);
    mesh1.addIndex(1);
    mesh1.addIndex(2);
    mesh1.addIndex(1);
    mesh1.addIndex(2);
    mesh1.addIndex(3);
    
    ofFill();
    mesh1.draw();
}

//--------------------------------------------------------------
glm::vec2 ofApp::perpen(glm::vec2 P){
    //垂直、反時計回りに９０度
    return glm::vec2(P.y, -P.x);
}
//--------------------------------------------------------------
glm::vec2 ofApp::inverse(glm::vec2 P){
    //１８０度反転
    return glm::vec2(-P.x, -P.y);
}
//--------------------------------------------------------------
void ofApp::drawCap_round_first(glm::vec2 p0, glm::vec2 p1, float _lineWidth){
    //最初のキャップ
    
    vector<glm::vec2> A;
    float half_lineWidth = _lineWidth/2;
    
    float angle1 = atan2(p1.y-p0.y, p1.x-p0.x);
    if (angle1 < 0) {angle1 = TWO_PI + angle1;}
    angle1 = angle1+HALF_PI;
    for (float ang = angle1; ang <= angle1 + PI; ang += PI/18) {
        float x = p0.x + (half_lineWidth * cos(ang));
        float y = p0.y + (half_lineWidth * sin(ang));
        A.push_back(glm::vec2(x, y));
    }
    
    ofMesh mesh;
    mesh.addVertex(ofPoint(p0.x, p0.y));
    for (int i = 0; i < A.size(); i++) {
        mesh.addVertex(ofPoint(A[i].x, A[i].y, 0));
    }
    
    for (int i = 1; i < A.size(); i++) {
        mesh.addIndex(0);
        mesh.addIndex(i);
        mesh.addIndex(i+1);
    }
    
    ofFill();
    mesh.draw();
}
//--------------------------------------------------------------
void ofApp::drawCap_round_last(glm::vec2 p0, glm::vec2 p1, float _lineWidth){
    //最後のキャップ
    vector<glm::vec2> A;
    float half_lineWidth = _lineWidth/2;
    
    float angle2 = atan2(p0.y-p1.y, p0.x-p1.x);
    if (angle2 < 0) {angle2 = TWO_PI + angle2;}
    angle2 = angle2 + HALF_PI;
    for (float ang = angle2; ang <= angle2+PI; ang += PI/18) {
        float x = p1.x + (half_lineWidth * cos(ang));
        float y = p1.y + (half_lineWidth * sin(ang));
        A.push_back(glm::vec2(x, y));
    }
    ofMesh mesh;
    mesh.addVertex(ofPoint(p1.x, p1.y));
    for (int i = 0; i < A.size(); i++) {
        mesh.addVertex(ofPoint(A[i].x, A[i].y, 0));
    }
    
    for (int i = 1; i < A.size(); i++) {
        mesh.addIndex(0);
        mesh.addIndex(i);
        mesh.addIndex(i+1);
    }
    
    //ofSetColor(col);
    ofFill();
    mesh.draw();
    
}

//--------------------------------------------------------------
//-------------------------easing-------------------------------
//--------------------------------------------------------------

// based on http://aa-deb.hatenablog.com/entry/2016/11/22/140000
// https://easings.net/ja

float ofApp::easeInSine(float t, float b, float c, float d){
  return -c * cos(t / d * (PI / 2)) + c + b;
}
//--------------------------------------------------------------
float ofApp::easeOutSine(float t, float b, float c, float d){
  return c * sin(t / d * (PI / 2)) + b;
}
//--------------------------------------------------------------
float ofApp::easeInOutSine(float t, float b, float c, float d){
  return -c / 2 * (cos(PI * t / d) - 1) + b;
}
//--------------------------------------------------------------
float ofApp::easeInQuad(float t, float b, float c, float d){
  t /= d;
  return c * t * t + b;
}
//--------------------------------------------------------------
float ofApp::easeOutQuad(float t, float b, float c, float d){
  t /= d;
  return -c * t * (t - 2) + b;
}
//--------------------------------------------------------------
float ofApp::easeInOutQuad(float t, float b, float c, float d){
  t /= d / 2;
  if(t < 1)
    return c / 2 * t * t + b;
  t--;
  return -c / 2 * (t * (t - 2) - 1) + b;
}
//--------------------------------------------------------------
float ofApp::easeInCubic(float t, float b, float c, float d){
  t /= d;
  return c * t * t * t + b;
}
//--------------------------------------------------------------
float ofApp::easeOutCubic(float t, float b, float c, float d){
  t /= d;
  t--;
  return c * (t * t * t + 1) + b;
}
//--------------------------------------------------------------
float ofApp::easeInOutCubic(float t, float b, float c, float d){
  t /= d / 2;
  if(t < 1)
    return c / 2 * t * t * t + b;
  t -= 2;
  return c / 2 * (t * t * t + 2) + b;
}
//--------------------------------------------------------------
float ofApp::easeInQuart(float t, float b, float c, float d){
  t /= d;
  return c * t * t * t * t + b;
}
//--------------------------------------------------------------
float ofApp::easeOutQuart(float t, float b, float c, float d){
  t /= d;
  t--;
  return -c * (t * t * t * t - 1) + b;
}
//--------------------------------------------------------------
float ofApp::easeInOutQuart(float t, float b, float c, float d){
  t /= d / 2;
  if(t < 1)
    return c / 2 * t * t * t * t + b;
  t -= 2;
  return -c / 2 * (t * t * t * t - 2) + b;
}
//--------------------------------------------------------------
float ofApp::easeInQuint(float t, float b, float c, float d){
  t /= d;
  return c * t * t * t * t * t + b;
}
//--------------------------------------------------------------
float ofApp::easeOutQuint(float t, float b, float c, float d){
  t /= d;
  t--;
  return c * (t * t * t * t  * t + 1) + b;
}
//--------------------------------------------------------------
float ofApp::easeInOutQuint(float t, float b, float c, float d){
  t /= d / 2;
  if(t < 1)
    return c / 2 * t * t * t * t * t + b;
  t -= 2;
  return c / 2 * (t * t * t * t * t + 2) + b;
}
//--------------------------------------------------------------
float ofApp::easeInExpo(float t, float b, float c, float d){
  return t == 0 ? b: c * pow(2, 10 * (t / d - 1)) + b;
}
//--------------------------------------------------------------
float ofApp::easeOutExpo(float t, float b, float c, float d){
  return t == d ? b + c: c * (-pow(2, -10 * t / d) + 1) + b;
}
//--------------------------------------------------------------
float ofApp::easeInOutExpo(float t, float b, float c, float d){
  if(t == 0)
    return b;
  if(t == d)
    return b + c;
  t /= d / 2;
  if(t < 1)
    return c / 2 * pow(2, 10 * (t - 1)) + b;
  t--;
  return c / 2 * (-pow(2, -10 * t) + 2) + b;
}
//--------------------------------------------------------------
float ofApp::easeInCirc(float t, float b, float c, float d){
  t /= d;
  return -c * (sqrt(1 - t * t) - 1) + b;
}
//--------------------------------------------------------------
float ofApp::easeOutCirc(float t, float b, float c, float d){
  t /= d;
  t--;
  return c * sqrt(1 - t * t) + b;
}
//--------------------------------------------------------------
float ofApp::easeInOutCirc(float t, float b, float c, float d){
  t /= d / 2;
  if(t < 1)
    return -c / 2 * (sqrt(1 - t * t) - 1) + b;
  t -= 2;
  return c / 2 * (sqrt(1 - t * t) + 1) + b;
}
//--------------------------------------------------------------
float ofApp::easeInBack(float t, float b, float c, float d, float s){
  t /= d;
  return c * t * t * ((s + 1) * t - s) + b;
}
//--------------------------------------------------------------
float ofApp::easeOutBack(float t, float b, float c, float d, float s){
  t /= d;
  t--;
  return c * (t * t * ((s + 1) * t + s) + 1) + b;
}
//--------------------------------------------------------------
float ofApp::easeInOutBack(float t, float b, float c, float d, float s){
  t /= d / 2;
  s *= 1.525;
  if(t < 1)
    return c / 2 * (t * t * ((s + 1) * t - s)) + b;
  t -= 2;
   return c / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
}
//--------------------------------------------------------------
float ofApp::easeInElastic(float t, float b, float c, float d){
  float s = 1.70158;
  float a = c;
  if(t == 0)
    return b;
  t /= d;
  if(t == 1)
    return b + c;
  float p = d * 0.3;
  if(a < abs(c)){
    a = c;
    s = p / 4;
  } else {
    s = p / (2 * PI) * asin(c / a);
  }
  t--;
  return -(a * pow(2, 10 * t) * sin((t * d - s) * (2 * PI) / p)) + b;
}
//--------------------------------------------------------------
float ofApp::easeOutElastic(float t, float b, float c, float d){
  float s = 1.70158;
  float a = c;
  if(t == 0)
    return b;
  t /= d;
  if(t == 1)
    return b + c;
  float p = d * 0.3;
  if(a < abs(c)){
    a = c;
    s = p / 4;
  } else {
    s = p / (2 * PI) * asin(c / a);
  }
  return a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p) + c + b;
}
//--------------------------------------------------------------
float ofApp::easeInOutElastic(float t, float b, float c, float d){
  float s = 1.70158;
  float a = c;
  if(t == 0)
    return b;
  t /= d / 2;
  if(t == 2)
    return b + c;
  float p = d * (0.3 * 1.5);
  if(a < abs(c)){
    a = c;
    s = p / 4;
  } else {
    s = p / (2 * PI) * asin(c / a);
  }
  if(t < 1){
    t--;
    return -0.5 * (a * pow(2, 10 * t) * sin((t * d - s) * (2 * PI) / p)) + b;
  }
  t--;
  return a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p) * 0.5 + c + b;
}
//--------------------------------------------------------------
float ofApp::easeInBounce(float t, float b, float c, float d){
  return c - easeOutBounce(d - t, 0, c, d) + b;
}
//--------------------------------------------------------------
float ofApp::easeOutBounce(float t, float b, float c, float d){
  t /= d;
  if(t < 1.0 / 2.75){
    return c * (7.5265 * t * t) + b;
  } else if(t < 2.0 / 2.75){
    t -= 1.5 / 2.75;
    return c * (7.5265 * t * t + 0.75) + b;
  } else if(t < 2.5 / 2.75){
    t -= 2.25 / 2.75;
    return c * (7.5265 * t * t + 0.9375) + b;
  }else{
    t -= 2.625 / 2.75;
    return c * (7.5265 * t * t + 0.984375) + b;
  }
}
//--------------------------------------------------------------
float ofApp::easeInOutBounce(float t, float b, float c, float d){
  if(t < d / 2)
    return easeInBounce(t * 2, 0, c, d) * 0.5 + b;
  return easeOutBounce(t * 2 - d, 0, c, d) * 0.5 + c * 0.5 + b;
}
//--------------------------------------------------------------
