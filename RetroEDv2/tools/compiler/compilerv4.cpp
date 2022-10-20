#include "includes.hpp"
#include "compilerv4.hpp"

#include "tools/sceneeditor.hpp"

Compilerv4::VariableInfo Compilerv4::variableNames_rev00[] = {
    // Internal Script Values
    VariableInfo("temp0"),
    VariableInfo("temp1"),
    VariableInfo("temp2"),
    VariableInfo("temp3"),
    VariableInfo("temp4"),
    VariableInfo("temp5"),
    VariableInfo("temp6"),
    VariableInfo("temp7"),
    VariableInfo("checkResult"),
    VariableInfo("arrayPos0"),
    VariableInfo("arrayPos1"),
    VariableInfo("arrayPos2"),
    VariableInfo("arrayPos3"),
    VariableInfo("arrayPos4"),
    VariableInfo("arrayPos5"),
    VariableInfo("arrayPos6"),
    VariableInfo("arrayPos7"),
    VariableInfo("global"),
    VariableInfo("local"),

    // Object Properties
    VariableInfo("object.entityPos"),
    VariableInfo("object.groupID"),
    VariableInfo("object.type"),
    VariableInfo("object.propertyValue"),
    VariableInfo("object.xpos"),
    VariableInfo("object.ypos"),
    VariableInfo("object.ixpos"),
    VariableInfo("object.iypos"),
    VariableInfo("object.xvel"),
    VariableInfo("object.yvel"),
    VariableInfo("object.speed"),
    VariableInfo("object.state"),
    VariableInfo("object.rotation"),
    VariableInfo("object.scale"),
    VariableInfo("object.priority"),
    VariableInfo("object.drawOrder"),
    VariableInfo("object.direction"),
    VariableInfo("object.inkEffect"),
    VariableInfo("object.alpha"),
    VariableInfo("object.frame"),
    VariableInfo("object.animation"),
    VariableInfo("object.prevAnimation"),
    VariableInfo("object.animationSpeed"),
    VariableInfo("object.animationTimer"),
    VariableInfo("object.angle"),
    VariableInfo("object.lookPosX"),
    VariableInfo("object.lookPosY"),
    VariableInfo("object.collisionMode"),
    VariableInfo("object.collisionPlane"),
    VariableInfo("object.controlMode"),
    VariableInfo("object.controlLock"),
    VariableInfo("object.pushing"),
    VariableInfo("object.visible"),
    VariableInfo("object.tileCollisions"),
    VariableInfo("object.interaction"),
    VariableInfo("object.gravity"),
    VariableInfo("object.up"),
    VariableInfo("object.down"),
    VariableInfo("object.left"),
    VariableInfo("object.right"),
    VariableInfo("object.jumpPress"),
    VariableInfo("object.jumpHold"),
    VariableInfo("object.scrollTracking"),
    VariableInfo("object.floorSensorL"),
    VariableInfo("object.floorSensorC"),
    VariableInfo("object.floorSensorR"),
    VariableInfo("object.collisionLeft"),
    VariableInfo("object.collisionTop"),
    VariableInfo("object.collisionRight"),
    VariableInfo("object.collisionBottom"),
    VariableInfo("object.outOfBounds"),
    VariableInfo("object.spriteSheet"),

    // Object Values
    VariableInfo("object.value0"),
    VariableInfo("object.value1"),
    VariableInfo("object.value2"),
    VariableInfo("object.value3"),
    VariableInfo("object.value4"),
    VariableInfo("object.value5"),
    VariableInfo("object.value6"),
    VariableInfo("object.value7"),
    VariableInfo("object.value8"),
    VariableInfo("object.value9"),
    VariableInfo("object.value10"),
    VariableInfo("object.value11"),
    VariableInfo("object.value12"),
    VariableInfo("object.value13"),
    VariableInfo("object.value14"),
    VariableInfo("object.value15"),
    VariableInfo("object.value16"),
    VariableInfo("object.value17"),
    VariableInfo("object.value18"),
    VariableInfo("object.value19"),
    VariableInfo("object.value20"),
    VariableInfo("object.value21"),
    VariableInfo("object.value22"),
    VariableInfo("object.value23"),
    VariableInfo("object.value24"),
    VariableInfo("object.value25"),
    VariableInfo("object.value26"),
    VariableInfo("object.value27"),
    VariableInfo("object.value28"),
    VariableInfo("object.value29"),
    VariableInfo("object.value30"),
    VariableInfo("object.value31"),
    VariableInfo("object.value32"),
    VariableInfo("object.value33"),
    VariableInfo("object.value34"),
    VariableInfo("object.value35"),
    VariableInfo("object.value36"),
    VariableInfo("object.value37"),
    VariableInfo("object.value38"),
    VariableInfo("object.value39"),
    VariableInfo("object.value40"),
    VariableInfo("object.value41"),
    VariableInfo("object.value42"),
    VariableInfo("object.value43"),
    VariableInfo("object.value44"),
    VariableInfo("object.value45"),
    VariableInfo("object.value46"),
    VariableInfo("object.value47"),

    // Stage Properties
    VariableInfo("stage.state"),
    VariableInfo("stage.activeList"),
    VariableInfo("stage.listPos"),
    VariableInfo("stage.timeEnabled"),
    VariableInfo("stage.milliSeconds"),
    VariableInfo("stage.seconds"),
    VariableInfo("stage.minutes"),
    VariableInfo("stage.actNum"),
    VariableInfo("stage.pauseEnabled"),
    VariableInfo("stage.listSize"),
    VariableInfo("stage.newXBoundary1"),
    VariableInfo("stage.newXBoundary2"),
    VariableInfo("stage.newYBoundary1"),
    VariableInfo("stage.newYBoundary2"),
    VariableInfo("stage.curXBoundary1"),
    VariableInfo("stage.curXBoundary2"),
    VariableInfo("stage.curYBoundary1"),
    VariableInfo("stage.curYBoundary2"),
    VariableInfo("stage.deformationData0"),
    VariableInfo("stage.deformationData1"),
    VariableInfo("stage.deformationData2"),
    VariableInfo("stage.deformationData3"),
    VariableInfo("stage.waterLevel"),
    VariableInfo("stage.activeLayer"),
    VariableInfo("stage.midPoint"),
    VariableInfo("stage.playerListPos"),
    VariableInfo("stage.debugMode"),
    VariableInfo("stage.entityPos"),

    // Screen Properties
    VariableInfo("screen.cameraEnabled"),
    VariableInfo("screen.cameraTarget"),
    VariableInfo("screen.cameraStyle"),
    VariableInfo("screen.cameraX"),
    VariableInfo("screen.cameraY"),
    VariableInfo("screen.drawListSize"),
    VariableInfo("screen.xcenter"),
    VariableInfo("screen.ycenter"),
    VariableInfo("screen.xsize"),
    VariableInfo("screen.ysize"),
    VariableInfo("screen.xoffset"),
    VariableInfo("screen.yoffset"),
    VariableInfo("screen.shakeX"),
    VariableInfo("screen.shakeY"),
    VariableInfo("screen.adjustCameraY"),

    VariableInfo("touchscreen.down"),
    VariableInfo("touchscreen.xpos"),
    VariableInfo("touchscreen.ypos"),

    // Sound Properties
    VariableInfo("music.volume"),
    VariableInfo("music.currentTrack"),
    VariableInfo("music.position"),

    // Input Properties
    VariableInfo("keyDown.up"),
    VariableInfo("keyDown.down"),
    VariableInfo("keyDown.left"),
    VariableInfo("keyDown.right"),
    VariableInfo("keyDown.buttonA"),
    VariableInfo("keyDown.buttonB"),
    VariableInfo("keyDown.buttonC"),
    VariableInfo("keyDown.buttonX"),
    VariableInfo("keyDown.buttonY"),
    VariableInfo("keyDown.buttonZ"),
    VariableInfo("keyDown.buttonL"),
    VariableInfo("keyDown.buttonR"),
    VariableInfo("keyDown.start"),
    VariableInfo("keyDown.select"),
    VariableInfo("keyPress.up"),
    VariableInfo("keyPress.down"),
    VariableInfo("keyPress.left"),
    VariableInfo("keyPress.right"),
    VariableInfo("keyPress.buttonA"),
    VariableInfo("keyPress.buttonB"),
    VariableInfo("keyPress.buttonC"),
    VariableInfo("keyPress.buttonX"),
    VariableInfo("keyPress.buttonY"),
    VariableInfo("keyPress.buttonZ"),
    VariableInfo("keyPress.buttonL"),
    VariableInfo("keyPress.buttonR"),
    VariableInfo("keyPress.start"),
    VariableInfo("keyPress.select"),

    // Menu Properties
    VariableInfo("menu1.selection"),
    VariableInfo("menu2.selection"),

    // Tile Layer Properties
    VariableInfo("tileLayer.xsize"),
    VariableInfo("tileLayer.ysize"),
    VariableInfo("tileLayer.type"),
    VariableInfo("tileLayer.angle"),
    VariableInfo("tileLayer.xpos"),
    VariableInfo("tileLayer.ypos"),
    VariableInfo("tileLayer.zpos"),
    VariableInfo("tileLayer.parallaxFactor"),
    VariableInfo("tileLayer.scrollSpeed"),
    VariableInfo("tileLayer.scrollPos"),
    VariableInfo("tileLayer.deformationOffset"),
    VariableInfo("tileLayer.deformationOffsetW"),
    VariableInfo("hParallax.parallaxFactor"),
    VariableInfo("hParallax.scrollSpeed"),
    VariableInfo("hParallax.scrollPos"),
    VariableInfo("vParallax.parallaxFactor"),
    VariableInfo("vParallax.scrollSpeed"),
    VariableInfo("vParallax.scrollPos"),

    // 3D Scene Properties
    VariableInfo("scene3D.vertexCount"),
    VariableInfo("scene3D.faceCount"),
    VariableInfo("scene3D.projectionX"),
    VariableInfo("scene3D.projectionY"),

    VariableInfo("vertexBuffer.x"),
    VariableInfo("vertexBuffer.y"),
    VariableInfo("vertexBuffer.z"),
    VariableInfo("vertexBuffer.u"),
    VariableInfo("vertexBuffer.v"),

    VariableInfo("faceBuffer.a"),
    VariableInfo("faceBuffer.b"),
    VariableInfo("faceBuffer.c"),
    VariableInfo("faceBuffer.d"),
    VariableInfo("faceBuffer.flag"),
    VariableInfo("faceBuffer.color"),

    VariableInfo("saveRAM"),
    VariableInfo("engine.state"),
    VariableInfo("engine.message"),
    VariableInfo("engine.language"),
    VariableInfo("engine.onlineActive"),
    VariableInfo("engine.sfxVolume"),
    VariableInfo("engine.bgmVolume"),
    VariableInfo("engine.platformID"),
    VariableInfo("engine.trialMode"),

    // Haptics
    VariableInfo("engine.hapticsEnabled"),

    // EDITOR-ONLY
    VariableInfo("editor.variableID"),
    VariableInfo("editor.variableValue"),
    VariableInfo("editor.returnVariable"),
    VariableInfo("editor.showGizmos"),
    VariableInfo("editor.drawingOverlay"),
};
Compilerv4::VariableInfo Compilerv4::variableNames_rev01[] = {
    // Internal Script Values
    VariableInfo("temp0"),
    VariableInfo("temp1"),
    VariableInfo("temp2"),
    VariableInfo("temp3"),
    VariableInfo("temp4"),
    VariableInfo("temp5"),
    VariableInfo("temp6"),
    VariableInfo("temp7"),
    VariableInfo("checkResult"),
    VariableInfo("arrayPos0"),
    VariableInfo("arrayPos1"),
    VariableInfo("arrayPos2"),
    VariableInfo("arrayPos3"),
    VariableInfo("arrayPos4"),
    VariableInfo("arrayPos5"),
    VariableInfo("arrayPos6"),
    VariableInfo("arrayPos7"),
    VariableInfo("global"),
    VariableInfo("local"),

    // Object Properties
    VariableInfo("object.entityPos"),
    VariableInfo("object.groupID"),
    VariableInfo("object.type"),
    VariableInfo("object.propertyValue"),
    VariableInfo("object.xpos"),
    VariableInfo("object.ypos"),
    VariableInfo("object.ixpos"),
    VariableInfo("object.iypos"),
    VariableInfo("object.xvel"),
    VariableInfo("object.yvel"),
    VariableInfo("object.speed"),
    VariableInfo("object.state"),
    VariableInfo("object.rotation"),
    VariableInfo("object.scale"),
    VariableInfo("object.priority"),
    VariableInfo("object.drawOrder"),
    VariableInfo("object.direction"),
    VariableInfo("object.inkEffect"),
    VariableInfo("object.alpha"),
    VariableInfo("object.frame"),
    VariableInfo("object.animation"),
    VariableInfo("object.prevAnimation"),
    VariableInfo("object.animationSpeed"),
    VariableInfo("object.animationTimer"),
    VariableInfo("object.angle"),
    VariableInfo("object.lookPosX"),
    VariableInfo("object.lookPosY"),
    VariableInfo("object.collisionMode"),
    VariableInfo("object.collisionPlane"),
    VariableInfo("object.controlMode"),
    VariableInfo("object.controlLock"),
    VariableInfo("object.pushing"),
    VariableInfo("object.visible"),
    VariableInfo("object.tileCollisions"),
    VariableInfo("object.interaction"),
    VariableInfo("object.gravity"),
    VariableInfo("object.up"),
    VariableInfo("object.down"),
    VariableInfo("object.left"),
    VariableInfo("object.right"),
    VariableInfo("object.jumpPress"),
    VariableInfo("object.jumpHold"),
    VariableInfo("object.scrollTracking"),
    VariableInfo("object.floorSensorL"),
    VariableInfo("object.floorSensorC"),
    VariableInfo("object.floorSensorR"),
    VariableInfo("object.floorSensorLC"),
    VariableInfo("object.floorSensorRC"),
    VariableInfo("object.collisionLeft"),
    VariableInfo("object.collisionTop"),
    VariableInfo("object.collisionRight"),
    VariableInfo("object.collisionBottom"),
    VariableInfo("object.outOfBounds"),
    VariableInfo("object.spriteSheet"),

    // Object Values
    VariableInfo("object.value0"),
    VariableInfo("object.value1"),
    VariableInfo("object.value2"),
    VariableInfo("object.value3"),
    VariableInfo("object.value4"),
    VariableInfo("object.value5"),
    VariableInfo("object.value6"),
    VariableInfo("object.value7"),
    VariableInfo("object.value8"),
    VariableInfo("object.value9"),
    VariableInfo("object.value10"),
    VariableInfo("object.value11"),
    VariableInfo("object.value12"),
    VariableInfo("object.value13"),
    VariableInfo("object.value14"),
    VariableInfo("object.value15"),
    VariableInfo("object.value16"),
    VariableInfo("object.value17"),
    VariableInfo("object.value18"),
    VariableInfo("object.value19"),
    VariableInfo("object.value20"),
    VariableInfo("object.value21"),
    VariableInfo("object.value22"),
    VariableInfo("object.value23"),
    VariableInfo("object.value24"),
    VariableInfo("object.value25"),
    VariableInfo("object.value26"),
    VariableInfo("object.value27"),
    VariableInfo("object.value28"),
    VariableInfo("object.value29"),
    VariableInfo("object.value30"),
    VariableInfo("object.value31"),
    VariableInfo("object.value32"),
    VariableInfo("object.value33"),
    VariableInfo("object.value34"),
    VariableInfo("object.value35"),
    VariableInfo("object.value36"),
    VariableInfo("object.value37"),
    VariableInfo("object.value38"),
    VariableInfo("object.value39"),
    VariableInfo("object.value40"),
    VariableInfo("object.value41"),
    VariableInfo("object.value42"),
    VariableInfo("object.value43"),
    VariableInfo("object.value44"),
    VariableInfo("object.value45"),
    VariableInfo("object.value46"),
    VariableInfo("object.value47"),

    // Stage Properties
    VariableInfo("stage.state"),
    VariableInfo("stage.activeList"),
    VariableInfo("stage.listPos"),
    VariableInfo("stage.timeEnabled"),
    VariableInfo("stage.milliSeconds"),
    VariableInfo("stage.seconds"),
    VariableInfo("stage.minutes"),
    VariableInfo("stage.actNum"),
    VariableInfo("stage.pauseEnabled"),
    VariableInfo("stage.listSize"),
    VariableInfo("stage.newXBoundary1"),
    VariableInfo("stage.newXBoundary2"),
    VariableInfo("stage.newYBoundary1"),
    VariableInfo("stage.newYBoundary2"),
    VariableInfo("stage.curXBoundary1"),
    VariableInfo("stage.curXBoundary2"),
    VariableInfo("stage.curYBoundary1"),
    VariableInfo("stage.curYBoundary2"),
    VariableInfo("stage.deformationData0"),
    VariableInfo("stage.deformationData1"),
    VariableInfo("stage.deformationData2"),
    VariableInfo("stage.deformationData3"),
    VariableInfo("stage.waterLevel"),
    VariableInfo("stage.activeLayer"),
    VariableInfo("stage.midPoint"),
    VariableInfo("stage.playerListPos"),
    VariableInfo("stage.debugMode"),
    VariableInfo("stage.entityPos"),

    // Screen Properties
    VariableInfo("screen.cameraEnabled"),
    VariableInfo("screen.cameraTarget"),
    VariableInfo("screen.cameraStyle"),
    VariableInfo("screen.cameraX"),
    VariableInfo("screen.cameraY"),
    VariableInfo("screen.drawListSize"),
    VariableInfo("screen.xcenter"),
    VariableInfo("screen.ycenter"),
    VariableInfo("screen.xsize"),
    VariableInfo("screen.ysize"),
    VariableInfo("screen.xoffset"),
    VariableInfo("screen.yoffset"),
    VariableInfo("screen.shakeX"),
    VariableInfo("screen.shakeY"),
    VariableInfo("screen.adjustCameraY"),

    VariableInfo("touchscreen.down"),
    VariableInfo("touchscreen.xpos"),
    VariableInfo("touchscreen.ypos"),

    // Sound Properties
    VariableInfo("music.volume"),
    VariableInfo("music.currentTrack"),
    VariableInfo("music.position"),

    // Input Properties
    VariableInfo("keyDown.up"),
    VariableInfo("keyDown.down"),
    VariableInfo("keyDown.left"),
    VariableInfo("keyDown.right"),
    VariableInfo("keyDown.buttonA"),
    VariableInfo("keyDown.buttonB"),
    VariableInfo("keyDown.buttonC"),
    VariableInfo("keyDown.buttonX"),
    VariableInfo("keyDown.buttonY"),
    VariableInfo("keyDown.buttonZ"),
    VariableInfo("keyDown.buttonL"),
    VariableInfo("keyDown.buttonR"),
    VariableInfo("keyDown.start"),
    VariableInfo("keyDown.select"),
    VariableInfo("keyPress.up"),
    VariableInfo("keyPress.down"),
    VariableInfo("keyPress.left"),
    VariableInfo("keyPress.right"),
    VariableInfo("keyPress.buttonA"),
    VariableInfo("keyPress.buttonB"),
    VariableInfo("keyPress.buttonC"),
    VariableInfo("keyPress.buttonX"),
    VariableInfo("keyPress.buttonY"),
    VariableInfo("keyPress.buttonZ"),
    VariableInfo("keyPress.buttonL"),
    VariableInfo("keyPress.buttonR"),
    VariableInfo("keyPress.start"),
    VariableInfo("keyPress.select"),

    // Menu Properties
    VariableInfo("menu1.selection"),
    VariableInfo("menu2.selection"),

    // Tile Layer Properties
    VariableInfo("tileLayer.xsize"),
    VariableInfo("tileLayer.ysize"),
    VariableInfo("tileLayer.type"),
    VariableInfo("tileLayer.angle"),
    VariableInfo("tileLayer.xpos"),
    VariableInfo("tileLayer.ypos"),
    VariableInfo("tileLayer.zpos"),
    VariableInfo("tileLayer.parallaxFactor"),
    VariableInfo("tileLayer.scrollSpeed"),
    VariableInfo("tileLayer.scrollPos"),
    VariableInfo("tileLayer.deformationOffset"),
    VariableInfo("tileLayer.deformationOffsetW"),
    VariableInfo("hParallax.parallaxFactor"),
    VariableInfo("hParallax.scrollSpeed"),
    VariableInfo("hParallax.scrollPos"),
    VariableInfo("vParallax.parallaxFactor"),
    VariableInfo("vParallax.scrollSpeed"),
    VariableInfo("vParallax.scrollPos"),

    // 3D Scene Properties
    VariableInfo("scene3D.vertexCount"),
    VariableInfo("scene3D.faceCount"),
    VariableInfo("scene3D.projectionX"),
    VariableInfo("scene3D.projectionY"),
    VariableInfo("scene3D.fogColor"),
    VariableInfo("scene3D.fogStrength"),

    VariableInfo("vertexBuffer.x"),
    VariableInfo("vertexBuffer.y"),
    VariableInfo("vertexBuffer.z"),
    VariableInfo("vertexBuffer.u"),
    VariableInfo("vertexBuffer.v"),

    VariableInfo("faceBuffer.a"),
    VariableInfo("faceBuffer.b"),
    VariableInfo("faceBuffer.c"),
    VariableInfo("faceBuffer.d"),
    VariableInfo("faceBuffer.flag"),
    VariableInfo("faceBuffer.color"),

    VariableInfo("saveRAM"),
    VariableInfo("engine.state"),
    VariableInfo("engine.language"),
    VariableInfo("engine.onlineActive"),
    VariableInfo("engine.sfxVolume"),
    VariableInfo("engine.bgmVolume"),
    VariableInfo("engine.trialMode"),
    VariableInfo("engine.deviceType"),

    // Haptics
    VariableInfo("engine.hapticsEnabled"),

    // EDITOR-ONLY
    VariableInfo("editor.variableID"),
    VariableInfo("editor.variableValue"),
    VariableInfo("editor.returnVariable"),
    VariableInfo("editor.showGizmos"),
    VariableInfo("editor.drawingOverlay"),
};
Compilerv4::VariableInfo Compilerv4::variableNames_rev02[] = {
    // Internal Script Values
    VariableInfo("temp0"),
    VariableInfo("temp1"),
    VariableInfo("temp2"),
    VariableInfo("temp3"),
    VariableInfo("temp4"),
    VariableInfo("temp5"),
    VariableInfo("temp6"),
    VariableInfo("temp7"),
    VariableInfo("checkResult"),
    VariableInfo("arrayPos0"),
    VariableInfo("arrayPos1"),
    VariableInfo("arrayPos2"),
    VariableInfo("arrayPos3"),
    VariableInfo("arrayPos4"),
    VariableInfo("arrayPos5"),
    VariableInfo("arrayPos6"),
    VariableInfo("arrayPos7"),
    VariableInfo("global"),
    VariableInfo("local"),

    // Object Properties
    VariableInfo("object.entityPos"),
    VariableInfo("object.groupID"),
    VariableInfo("object.type"),
    VariableInfo("object.propertyValue"),
    VariableInfo("object.xpos"),
    VariableInfo("object.ypos"),
    VariableInfo("object.ixpos"),
    VariableInfo("object.iypos"),
    VariableInfo("object.xvel"),
    VariableInfo("object.yvel"),
    VariableInfo("object.speed"),
    VariableInfo("object.state"),
    VariableInfo("object.rotation"),
    VariableInfo("object.scale"),
    VariableInfo("object.priority"),
    VariableInfo("object.drawOrder"),
    VariableInfo("object.direction"),
    VariableInfo("object.inkEffect"),
    VariableInfo("object.alpha"),
    VariableInfo("object.frame"),
    VariableInfo("object.animation"),
    VariableInfo("object.prevAnimation"),
    VariableInfo("object.animationSpeed"),
    VariableInfo("object.animationTimer"),
    VariableInfo("object.angle"),
    VariableInfo("object.lookPosX"),
    VariableInfo("object.lookPosY"),
    VariableInfo("object.collisionMode"),
    VariableInfo("object.collisionPlane"),
    VariableInfo("object.controlMode"),
    VariableInfo("object.controlLock"),
    VariableInfo("object.pushing"),
    VariableInfo("object.visible"),
    VariableInfo("object.tileCollisions"),
    VariableInfo("object.interaction"),
    VariableInfo("object.gravity"),
    VariableInfo("object.up"),
    VariableInfo("object.down"),
    VariableInfo("object.left"),
    VariableInfo("object.right"),
    VariableInfo("object.jumpPress"),
    VariableInfo("object.jumpHold"),
    VariableInfo("object.scrollTracking"),
    VariableInfo("object.floorSensorL"),
    VariableInfo("object.floorSensorC"),
    VariableInfo("object.floorSensorR"),
    VariableInfo("object.floorSensorLC"),
    VariableInfo("object.floorSensorRC"),
    VariableInfo("object.collisionLeft"),
    VariableInfo("object.collisionTop"),
    VariableInfo("object.collisionRight"),
    VariableInfo("object.collisionBottom"),
    VariableInfo("object.outOfBounds"),
    VariableInfo("object.spriteSheet"),

    // Object Values
    VariableInfo("object.value0"),
    VariableInfo("object.value1"),
    VariableInfo("object.value2"),
    VariableInfo("object.value3"),
    VariableInfo("object.value4"),
    VariableInfo("object.value5"),
    VariableInfo("object.value6"),
    VariableInfo("object.value7"),
    VariableInfo("object.value8"),
    VariableInfo("object.value9"),
    VariableInfo("object.value10"),
    VariableInfo("object.value11"),
    VariableInfo("object.value12"),
    VariableInfo("object.value13"),
    VariableInfo("object.value14"),
    VariableInfo("object.value15"),
    VariableInfo("object.value16"),
    VariableInfo("object.value17"),
    VariableInfo("object.value18"),
    VariableInfo("object.value19"),
    VariableInfo("object.value20"),
    VariableInfo("object.value21"),
    VariableInfo("object.value22"),
    VariableInfo("object.value23"),
    VariableInfo("object.value24"),
    VariableInfo("object.value25"),
    VariableInfo("object.value26"),
    VariableInfo("object.value27"),
    VariableInfo("object.value28"),
    VariableInfo("object.value29"),
    VariableInfo("object.value30"),
    VariableInfo("object.value31"),
    VariableInfo("object.value32"),
    VariableInfo("object.value33"),
    VariableInfo("object.value34"),
    VariableInfo("object.value35"),
    VariableInfo("object.value36"),
    VariableInfo("object.value37"),
    VariableInfo("object.value38"),
    VariableInfo("object.value39"),
    VariableInfo("object.value40"),
    VariableInfo("object.value41"),
    VariableInfo("object.value42"),
    VariableInfo("object.value43"),
    VariableInfo("object.value44"),
    VariableInfo("object.value45"),
    VariableInfo("object.value46"),
    VariableInfo("object.value47"),

    // Stage Properties
    VariableInfo("stage.state"),
    VariableInfo("stage.activeList"),
    VariableInfo("stage.listPos"),
    VariableInfo("stage.timeEnabled"),
    VariableInfo("stage.milliSeconds"),
    VariableInfo("stage.seconds"),
    VariableInfo("stage.minutes"),
    VariableInfo("stage.actNum"),
    VariableInfo("stage.pauseEnabled"),
    VariableInfo("stage.listSize"),
    VariableInfo("stage.newXBoundary1"),
    VariableInfo("stage.newXBoundary2"),
    VariableInfo("stage.newYBoundary1"),
    VariableInfo("stage.newYBoundary2"),
    VariableInfo("stage.curXBoundary1"),
    VariableInfo("stage.curXBoundary2"),
    VariableInfo("stage.curYBoundary1"),
    VariableInfo("stage.curYBoundary2"),
    VariableInfo("stage.deformationData0"),
    VariableInfo("stage.deformationData1"),
    VariableInfo("stage.deformationData2"),
    VariableInfo("stage.deformationData3"),
    VariableInfo("stage.waterLevel"),
    VariableInfo("stage.activeLayer"),
    VariableInfo("stage.midPoint"),
    VariableInfo("stage.playerListPos"),
    VariableInfo("stage.debugMode"),
    VariableInfo("stage.entityPos"),

    // Screen Properties
    VariableInfo("screen.cameraEnabled"),
    VariableInfo("screen.cameraTarget"),
    VariableInfo("screen.cameraStyle"),
    VariableInfo("screen.cameraX"),
    VariableInfo("screen.cameraY"),
    VariableInfo("screen.drawListSize"),
    VariableInfo("screen.xcenter"),
    VariableInfo("screen.ycenter"),
    VariableInfo("screen.xsize"),
    VariableInfo("screen.ysize"),
    VariableInfo("screen.xoffset"),
    VariableInfo("screen.yoffset"),
    VariableInfo("screen.shakeX"),
    VariableInfo("screen.shakeY"),
    VariableInfo("screen.adjustCameraY"),

    VariableInfo("touchscreen.down"),
    VariableInfo("touchscreen.xpos"),
    VariableInfo("touchscreen.ypos"),

    // Sound Properties
    VariableInfo("music.volume"),
    VariableInfo("music.currentTrack"),
    VariableInfo("music.position"),

    // Input Properties
    VariableInfo("keyDown.up"),
    VariableInfo("keyDown.down"),
    VariableInfo("keyDown.left"),
    VariableInfo("keyDown.right"),
    VariableInfo("keyDown.buttonA"),
    VariableInfo("keyDown.buttonB"),
    VariableInfo("keyDown.buttonC"),
    VariableInfo("keyDown.buttonX"),
    VariableInfo("keyDown.buttonY"),
    VariableInfo("keyDown.buttonZ"),
    VariableInfo("keyDown.buttonL"),
    VariableInfo("keyDown.buttonR"),
    VariableInfo("keyDown.start"),
    VariableInfo("keyDown.select"),
    VariableInfo("keyPress.up"),
    VariableInfo("keyPress.down"),
    VariableInfo("keyPress.left"),
    VariableInfo("keyPress.right"),
    VariableInfo("keyPress.buttonA"),
    VariableInfo("keyPress.buttonB"),
    VariableInfo("keyPress.buttonC"),
    VariableInfo("keyPress.buttonX"),
    VariableInfo("keyPress.buttonY"),
    VariableInfo("keyPress.buttonZ"),
    VariableInfo("keyPress.buttonL"),
    VariableInfo("keyPress.buttonR"),
    VariableInfo("keyPress.start"),
    VariableInfo("keyPress.select"),

    // Menu Properties
    VariableInfo("menu1.selection"),
    VariableInfo("menu2.selection"),

    // Tile Layer Properties
    VariableInfo("tileLayer.xsize"),
    VariableInfo("tileLayer.ysize"),
    VariableInfo("tileLayer.type"),
    VariableInfo("tileLayer.angle"),
    VariableInfo("tileLayer.xpos"),
    VariableInfo("tileLayer.ypos"),
    VariableInfo("tileLayer.zpos"),
    VariableInfo("tileLayer.parallaxFactor"),
    VariableInfo("tileLayer.scrollSpeed"),
    VariableInfo("tileLayer.scrollPos"),
    VariableInfo("tileLayer.deformationOffset"),
    VariableInfo("tileLayer.deformationOffsetW"),
    VariableInfo("hParallax.parallaxFactor"),
    VariableInfo("hParallax.scrollSpeed"),
    VariableInfo("hParallax.scrollPos"),
    VariableInfo("vParallax.parallaxFactor"),
    VariableInfo("vParallax.scrollSpeed"),
    VariableInfo("vParallax.scrollPos"),

    // 3D Scene Properties
    VariableInfo("scene3D.vertexCount"),
    VariableInfo("scene3D.faceCount"),
    VariableInfo("scene3D.projectionX"),
    VariableInfo("scene3D.projectionY"),
    VariableInfo("scene3D.fogColor"),
    VariableInfo("scene3D.fogStrength"),

    VariableInfo("vertexBuffer.x"),
    VariableInfo("vertexBuffer.y"),
    VariableInfo("vertexBuffer.z"),
    VariableInfo("vertexBuffer.u"),
    VariableInfo("vertexBuffer.v"),

    VariableInfo("faceBuffer.a"),
    VariableInfo("faceBuffer.b"),
    VariableInfo("faceBuffer.c"),
    VariableInfo("faceBuffer.d"),
    VariableInfo("faceBuffer.flag"),
    VariableInfo("faceBuffer.color"),

    VariableInfo("saveRAM"),
    VariableInfo("engine.state"),
    VariableInfo("engine.language"),
    VariableInfo("engine.onlineActive"),
    VariableInfo("engine.sfxVolume"),
    VariableInfo("engine.bgmVolume"),
    VariableInfo("engine.trialMode"),
    VariableInfo("engine.deviceType"),

    // Haptics
    VariableInfo("engine.hapticsEnabled"),

    // EDITOR-ONLY
    VariableInfo("editor.variableID"),
    VariableInfo("editor.variableValue"),
    VariableInfo("editor.returnVariable"),
    VariableInfo("editor.showGizmos"),
    VariableInfo("editor.drawingOverlay"),
};
Compilerv4::VariableInfo Compilerv4::variableNames_rev03[] = {
    // Internal Script Values
    VariableInfo("temp0"),
    VariableInfo("temp1"),
    VariableInfo("temp2"),
    VariableInfo("temp3"),
    VariableInfo("temp4"),
    VariableInfo("temp5"),
    VariableInfo("temp6"),
    VariableInfo("temp7"),
    VariableInfo("checkResult"),
    VariableInfo("arrayPos0"),
    VariableInfo("arrayPos1"),
    VariableInfo("arrayPos2"),
    VariableInfo("arrayPos3"),
    VariableInfo("arrayPos4"),
    VariableInfo("arrayPos5"),
    VariableInfo("arrayPos6"),
    VariableInfo("arrayPos7"),
    VariableInfo("global"),
    VariableInfo("local"),

    // Object Properties
    VariableInfo("object.entityPos"),
    VariableInfo("object.groupID"),
    VariableInfo("object.type"),
    VariableInfo("object.propertyValue"),
    VariableInfo("object.xpos"),
    VariableInfo("object.ypos"),
    VariableInfo("object.ixpos"),
    VariableInfo("object.iypos"),
    VariableInfo("object.xvel"),
    VariableInfo("object.yvel"),
    VariableInfo("object.speed"),
    VariableInfo("object.state"),
    VariableInfo("object.rotation"),
    VariableInfo("object.scale"),
    VariableInfo("object.priority"),
    VariableInfo("object.drawOrder"),
    VariableInfo("object.direction"),
    VariableInfo("object.inkEffect"),
    VariableInfo("object.alpha"),
    VariableInfo("object.frame"),
    VariableInfo("object.animation"),
    VariableInfo("object.prevAnimation"),
    VariableInfo("object.animationSpeed"),
    VariableInfo("object.animationTimer"),
    VariableInfo("object.angle"),
    VariableInfo("object.lookPosX"),
    VariableInfo("object.lookPosY"),
    VariableInfo("object.collisionMode"),
    VariableInfo("object.collisionPlane"),
    VariableInfo("object.controlMode"),
    VariableInfo("object.controlLock"),
    VariableInfo("object.pushing"),
    VariableInfo("object.visible"),
    VariableInfo("object.tileCollisions"),
    VariableInfo("object.interaction"),
    VariableInfo("object.gravity"),
    VariableInfo("object.up"),
    VariableInfo("object.down"),
    VariableInfo("object.left"),
    VariableInfo("object.right"),
    VariableInfo("object.jumpPress"),
    VariableInfo("object.jumpHold"),
    VariableInfo("object.scrollTracking"),
    VariableInfo("object.floorSensorL"),
    VariableInfo("object.floorSensorC"),
    VariableInfo("object.floorSensorR"),
    VariableInfo("object.floorSensorLC"),
    VariableInfo("object.floorSensorRC"),
    VariableInfo("object.collisionLeft"),
    VariableInfo("object.collisionTop"),
    VariableInfo("object.collisionRight"),
    VariableInfo("object.collisionBottom"),
    VariableInfo("object.outOfBounds"),
    VariableInfo("object.spriteSheet"),

    // Object Values
    VariableInfo("object.value0"),
    VariableInfo("object.value1"),
    VariableInfo("object.value2"),
    VariableInfo("object.value3"),
    VariableInfo("object.value4"),
    VariableInfo("object.value5"),
    VariableInfo("object.value6"),
    VariableInfo("object.value7"),
    VariableInfo("object.value8"),
    VariableInfo("object.value9"),
    VariableInfo("object.value10"),
    VariableInfo("object.value11"),
    VariableInfo("object.value12"),
    VariableInfo("object.value13"),
    VariableInfo("object.value14"),
    VariableInfo("object.value15"),
    VariableInfo("object.value16"),
    VariableInfo("object.value17"),
    VariableInfo("object.value18"),
    VariableInfo("object.value19"),
    VariableInfo("object.value20"),
    VariableInfo("object.value21"),
    VariableInfo("object.value22"),
    VariableInfo("object.value23"),
    VariableInfo("object.value24"),
    VariableInfo("object.value25"),
    VariableInfo("object.value26"),
    VariableInfo("object.value27"),
    VariableInfo("object.value28"),
    VariableInfo("object.value29"),
    VariableInfo("object.value30"),
    VariableInfo("object.value31"),
    VariableInfo("object.value32"),
    VariableInfo("object.value33"),
    VariableInfo("object.value34"),
    VariableInfo("object.value35"),
    VariableInfo("object.value36"),
    VariableInfo("object.value37"),
    VariableInfo("object.value38"),
    VariableInfo("object.value39"),
    VariableInfo("object.value40"),
    VariableInfo("object.value41"),
    VariableInfo("object.value42"),
    VariableInfo("object.value43"),
    VariableInfo("object.value44"),
    VariableInfo("object.value45"),
    VariableInfo("object.value46"),
    VariableInfo("object.value47"),

    // Stage Properties
    VariableInfo("stage.state"),
    VariableInfo("stage.activeList"),
    VariableInfo("stage.listPos"),
    VariableInfo("stage.timeEnabled"),
    VariableInfo("stage.milliSeconds"),
    VariableInfo("stage.seconds"),
    VariableInfo("stage.minutes"),
    VariableInfo("stage.actNum"),
    VariableInfo("stage.pauseEnabled"),
    VariableInfo("stage.listSize"),
    VariableInfo("stage.newXBoundary1"),
    VariableInfo("stage.newXBoundary2"),
    VariableInfo("stage.newYBoundary1"),
    VariableInfo("stage.newYBoundary2"),
    VariableInfo("stage.curXBoundary1"),
    VariableInfo("stage.curXBoundary2"),
    VariableInfo("stage.curYBoundary1"),
    VariableInfo("stage.curYBoundary2"),
    VariableInfo("stage.deformationData0"),
    VariableInfo("stage.deformationData1"),
    VariableInfo("stage.deformationData2"),
    VariableInfo("stage.deformationData3"),
    VariableInfo("stage.waterLevel"),
    VariableInfo("stage.activeLayer"),
    VariableInfo("stage.midPoint"),
    VariableInfo("stage.playerListPos"),
    VariableInfo("stage.debugMode"),
    VariableInfo("stage.entityPos"),

    // Screen Properties
    VariableInfo("screen.cameraEnabled"),
    VariableInfo("screen.cameraTarget"),
    VariableInfo("screen.cameraStyle"),
    VariableInfo("screen.cameraX"),
    VariableInfo("screen.cameraY"),
    VariableInfo("screen.drawListSize"),
    VariableInfo("screen.xcenter"),
    VariableInfo("screen.ycenter"),
    VariableInfo("screen.xsize"),
    VariableInfo("screen.ysize"),
    VariableInfo("screen.xoffset"),
    VariableInfo("screen.yoffset"),
    VariableInfo("screen.shakeX"),
    VariableInfo("screen.shakeY"),
    VariableInfo("screen.adjustCameraY"),

    VariableInfo("touchscreen.down"),
    VariableInfo("touchscreen.xpos"),
    VariableInfo("touchscreen.ypos"),

    // Sound Properties
    VariableInfo("music.volume"),
    VariableInfo("music.currentTrack"),
    VariableInfo("music.position"),

    // Input Properties
    VariableInfo("keyDown.up"),
    VariableInfo("keyDown.down"),
    VariableInfo("keyDown.left"),
    VariableInfo("keyDown.right"),
    VariableInfo("keyDown.buttonA"),
    VariableInfo("keyDown.buttonB"),
    VariableInfo("keyDown.buttonC"),
    VariableInfo("keyDown.buttonX"),
    VariableInfo("keyDown.buttonY"),
    VariableInfo("keyDown.buttonZ"),
    VariableInfo("keyDown.buttonL"),
    VariableInfo("keyDown.buttonR"),
    VariableInfo("keyDown.start"),
    VariableInfo("keyDown.select"),
    VariableInfo("keyPress.up"),
    VariableInfo("keyPress.down"),
    VariableInfo("keyPress.left"),
    VariableInfo("keyPress.right"),
    VariableInfo("keyPress.buttonA"),
    VariableInfo("keyPress.buttonB"),
    VariableInfo("keyPress.buttonC"),
    VariableInfo("keyPress.buttonX"),
    VariableInfo("keyPress.buttonY"),
    VariableInfo("keyPress.buttonZ"),
    VariableInfo("keyPress.buttonL"),
    VariableInfo("keyPress.buttonR"),
    VariableInfo("keyPress.start"),
    VariableInfo("keyPress.select"),

    // Menu Properties
    VariableInfo("menu1.selection"),
    VariableInfo("menu2.selection"),

    // Tile Layer Properties
    VariableInfo("tileLayer.xsize"),
    VariableInfo("tileLayer.ysize"),
    VariableInfo("tileLayer.type"),
    VariableInfo("tileLayer.angle"),
    VariableInfo("tileLayer.xpos"),
    VariableInfo("tileLayer.ypos"),
    VariableInfo("tileLayer.zpos"),
    VariableInfo("tileLayer.parallaxFactor"),
    VariableInfo("tileLayer.scrollSpeed"),
    VariableInfo("tileLayer.scrollPos"),
    VariableInfo("tileLayer.deformationOffset"),
    VariableInfo("tileLayer.deformationOffsetW"),
    VariableInfo("hParallax.parallaxFactor"),
    VariableInfo("hParallax.scrollSpeed"),
    VariableInfo("hParallax.scrollPos"),
    VariableInfo("vParallax.parallaxFactor"),
    VariableInfo("vParallax.scrollSpeed"),
    VariableInfo("vParallax.scrollPos"),

    // 3D Scene Properties
    VariableInfo("scene3D.vertexCount"),
    VariableInfo("scene3D.faceCount"),
    VariableInfo("scene3D.projectionX"),
    VariableInfo("scene3D.projectionY"),
    VariableInfo("scene3D.fogColor"),
    VariableInfo("scene3D.fogStrength"),

    VariableInfo("vertexBuffer.x"),
    VariableInfo("vertexBuffer.y"),
    VariableInfo("vertexBuffer.z"),
    VariableInfo("vertexBuffer.u"),
    VariableInfo("vertexBuffer.v"),

    VariableInfo("faceBuffer.a"),
    VariableInfo("faceBuffer.b"),
    VariableInfo("faceBuffer.c"),
    VariableInfo("faceBuffer.d"),
    VariableInfo("faceBuffer.flag"),
    VariableInfo("faceBuffer.color"),

    VariableInfo("saveRAM"),
    VariableInfo("engine.state"),
    VariableInfo("engine.language"),
    VariableInfo("engine.onlineActive"),
    VariableInfo("engine.sfxVolume"),
    VariableInfo("engine.bgmVolume"),
    VariableInfo("engine.trialMode"),
    VariableInfo("engine.deviceType"),

    // Extras
    VariableInfo("screen.currentID"),
    VariableInfo("camera.enabled"),
    VariableInfo("camera.target"),
    VariableInfo("camera.style"),
    VariableInfo("camera.xpos"),
    VariableInfo("camera.ypos"),
    VariableInfo("camera.adjustY"),

    // Haptics
    VariableInfo("engine.hapticsEnabled"),

    // EDITOR-ONLY
    VariableInfo("editor.variableID"),
    VariableInfo("editor.variableValue"),
    VariableInfo("editor.returnVariable"),
    VariableInfo("editor.showGizmos"),
    VariableInfo("editor.drawingOverlay"),
};

Compilerv4::FunctionInfo Compilerv4::functionList_rev00[] = {
    FunctionInfo("End", 0),      // End of Script
    FunctionInfo("Equal", 2),    // Equal
    FunctionInfo("Add", 2),      // Add
    FunctionInfo("Sub", 2),      // Subtract
    FunctionInfo("Inc", 1),      // Increment
    FunctionInfo("Dec", 1),      // Decrement
    FunctionInfo("Mul", 2),      // Multiply
    FunctionInfo("Div", 2),      // Divide
    FunctionInfo("ShR", 2),      // Bit Shift Right
    FunctionInfo("ShL", 2),      // Bit Shift Left
    FunctionInfo("And", 2),      // Bitwise And
    FunctionInfo("Or", 2),       // Bitwise Or
    FunctionInfo("Xor", 2),      // Bitwise Xor
    FunctionInfo("Mod", 2),      // Mod
    FunctionInfo("FlipSign", 1), // Flips the Sign of the value

    FunctionInfo("CheckEqual", 2),    // compare a=b, return result in CheckResult Variable
    FunctionInfo("CheckGreater", 2),  // compare a>b, return result in CheckResult Variable
    FunctionInfo("CheckLower", 2),    // compare a<b, return result in CheckResult Variable
    FunctionInfo("CheckNotEqual", 2), // compare a!=b, return result in CheckResult Variable

    FunctionInfo("IfEqual", 3),          // compare a=b, jump if condition met
    FunctionInfo("IfGreater", 3),        // compare a>b, jump if condition met
    FunctionInfo("IfGreaterOrEqual", 3), // compare a>=b, jump if condition met
    FunctionInfo("IfLower", 3),          // compare a<b, jump if condition met
    FunctionInfo("IfLowerOrEqual", 3),   // compare a<=b, jump if condition met
    FunctionInfo("IfNotEqual", 3),       // compare a!=b, jump if condition met
    FunctionInfo("else", 0),             // The else for an if statement
    FunctionInfo("endif", 0),            // The end if

    FunctionInfo("WEqual", 3),          // compare a=b, loop if condition met
    FunctionInfo("WGreater", 3),        // compare a>b, loop if condition met
    FunctionInfo("WGreaterOrEqual", 3), // compare a>=b, loop if condition met
    FunctionInfo("WLower", 3),          // compare a<b, loop if condition met
    FunctionInfo("WLowerOrEqual", 3),   // compare a<=b, loop if condition met
    FunctionInfo("WNotEqual", 3),       // compare a!=b, loop if condition met
    FunctionInfo("loop", 0),            // While Loop marker

    FunctionInfo("ForEachActive", 3), // foreach loop, iterates through object group lists only if they
                                      // are active and interaction is true
    FunctionInfo("ForEachAll", 3),    // foreach loop, iterates through objects matching type
    FunctionInfo("next", 0),          // foreach loop, next marker

    FunctionInfo("switch", 2),    // Switch Statement
    FunctionInfo("break", 0),     // break
    FunctionInfo("endswitch", 0), // endswitch

    // Math Functions
    FunctionInfo("Rand", 2),
    FunctionInfo("Sin", 2),
    FunctionInfo("Cos", 2),
    FunctionInfo("Sin256", 2),
    FunctionInfo("Cos256", 2),
    FunctionInfo("ATan2", 3),
    FunctionInfo("Interpolate", 4),
    FunctionInfo("InterpolateXY", 7),

    // Graphics Functions
    FunctionInfo("LoadSpriteSheet", 1),
    FunctionInfo("RemoveSpriteSheet", 1),
    FunctionInfo("DrawSprite", 1),
    FunctionInfo("DrawSpriteXY", 3),
    FunctionInfo("DrawSpriteScreenXY", 3),
    FunctionInfo("DrawTintRect", 4),
    FunctionInfo("DrawNumbers", 7),
    FunctionInfo("DrawActName", 7),
    FunctionInfo("DrawMenu", 3),
    FunctionInfo("SpriteFrame", 6),
    FunctionInfo("EditFrame", 7),
    FunctionInfo("LoadPalette", 5),
    FunctionInfo("RotatePalette", 4),
    FunctionInfo("SetScreenFade", 4),
    FunctionInfo("SetActivePalette", 3),
    FunctionInfo("SetPaletteFade", 7),
    FunctionInfo("SetPaletteEntry", 3),
    FunctionInfo("GetPaletteEntry", 3),
    FunctionInfo("CopyPalette", 5),
    FunctionInfo("ClearScreen", 1),
    FunctionInfo("DrawSpriteFX", 4),
    FunctionInfo("DrawSpriteScreenFX", 4),

    // More Useful Stuff
    FunctionInfo("LoadAnimation", 1),
    FunctionInfo("SetupMenu", 4),
    FunctionInfo("AddMenuEntry", 3),
    FunctionInfo("EditMenuEntry", 4),
    FunctionInfo("LoadStage", 0),
    FunctionInfo("DrawRect", 8),
    FunctionInfo("ResetObjectEntity", 5),
    FunctionInfo("BoxCollisionTest", 11),
    FunctionInfo("CreateTempObject", 4),

    // Player and Animation Functions
    FunctionInfo("ProcessObjectMovement", 0),
    FunctionInfo("ProcessObjectControl", 0),
    FunctionInfo("ProcessAnimation", 0),
    FunctionInfo("DrawObjectAnimation", 0),

    // Music
    FunctionInfo("SetMusicTrack", 3),
    FunctionInfo("PlayMusic", 1),
    FunctionInfo("StopMusic", 0),
    FunctionInfo("PauseMusic", 0),
    FunctionInfo("ResumeMusic", 0),
    FunctionInfo("SwapMusicTrack", 4),

    // Sound FX
    FunctionInfo("PlaySfx", 2),
    FunctionInfo("StopSfx", 1),
    FunctionInfo("SetSfxAttributes", 3),

    // More Collision Stuff
    FunctionInfo("ObjectTileCollision", 4),
    FunctionInfo("ObjectTileGrip", 4),

    // Bitwise Not
    FunctionInfo("Not", 1),

    // 3D Stuff
    FunctionInfo("Draw3DScene", 0),
    FunctionInfo("SetIdentityMatrix", 1),
    FunctionInfo("MatrixMultiply", 2),
    FunctionInfo("MatrixTranslateXYZ", 4),
    FunctionInfo("MatrixScaleXYZ", 4),
    FunctionInfo("MatrixRotateX", 2),
    FunctionInfo("MatrixRotateY", 2),
    FunctionInfo("MatrixRotateZ", 2),
    FunctionInfo("MatrixRotateXYZ", 4),
    FunctionInfo("TransformVertices", 3),

    FunctionInfo("CallFunction", 1),
    FunctionInfo("return", 0),

    FunctionInfo("SetLayerDeformation", 6),
    FunctionInfo("CheckTouchRect", 4),
    FunctionInfo("GetTileLayerEntry", 4),
    FunctionInfo("SetTileLayerEntry", 4),

    FunctionInfo("GetBit", 3),
    FunctionInfo("SetBit", 3),

    FunctionInfo("ClearDrawList", 1),
    FunctionInfo("AddDrawListEntityRef", 2),
    FunctionInfo("GetDrawListEntityRef", 3),
    FunctionInfo("SetDrawListEntityRef", 3),

    FunctionInfo("Get16x16TileInfo", 4),
    FunctionInfo("Set16x16TileInfo", 4),
    FunctionInfo("Copy16x16Tile", 2),
    FunctionInfo("GetAnimationByName", 2),
    FunctionInfo("ReadSaveRAM", 0),
    FunctionInfo("WriteSaveRAM", 0),

    FunctionInfo("LoadFontFile", 1),
    FunctionInfo("LoadTextFile", 3),
    FunctionInfo("GetTextInfo", 5),
    FunctionInfo("DrawText", 7),
    FunctionInfo("GetVersionNumber", 2),

    FunctionInfo("GetTableValue", 3),
    FunctionInfo("SetTableValue", 3),

    FunctionInfo("CheckCurrentStageFolder", 1),
    FunctionInfo("Abs", 1),

    FunctionInfo("CallNativeFunction", 1),
    FunctionInfo("CallNativeFunction2", 3),
    FunctionInfo("CallNativeFunction4", 5),

    FunctionInfo("SetObjectRange", 1),
    FunctionInfo("Print", 3),

    // EDITOR-ONLY
    FunctionInfo("AddEditorVariable", 1),
    FunctionInfo("SetActiveVariable", 1),
    FunctionInfo("AddEnumVariable", 2),
    FunctionInfo("SetVariableAlias", 2),
    FunctionInfo("DrawLine", 7),
    FunctionInfo("DrawArrow", 7),
    FunctionInfo("DrawRectWorld", 8),
    FunctionInfo("DrawRectOutline", 8),
    FunctionInfo("GetObjectType", 2),
};
Compilerv4::FunctionInfo Compilerv4::functionList_rev01[] = {

    FunctionInfo("End", 0),      // End of Script
    FunctionInfo("Equal", 2),    // Equal
    FunctionInfo("Add", 2),      // Add
    FunctionInfo("Sub", 2),      // Subtract
    FunctionInfo("Inc", 1),      // Increment
    FunctionInfo("Dec", 1),      // Decrement
    FunctionInfo("Mul", 2),      // Multiply
    FunctionInfo("Div", 2),      // Divide
    FunctionInfo("ShR", 2),      // Bit Shift Right
    FunctionInfo("ShL", 2),      // Bit Shift Left
    FunctionInfo("And", 2),      // Bitwise And
    FunctionInfo("Or", 2),       // Bitwise Or
    FunctionInfo("Xor", 2),      // Bitwise Xor
    FunctionInfo("Mod", 2),      // Mod
    FunctionInfo("FlipSign", 1), // Flips the Sign of the value

    FunctionInfo("CheckEqual", 2),    // compare a=b, return result in CheckResult Variable
    FunctionInfo("CheckGreater", 2),  // compare a>b, return result in CheckResult Variable
    FunctionInfo("CheckLower", 2),    // compare a<b, return result in CheckResult Variable
    FunctionInfo("CheckNotEqual", 2), // compare a!=b, return result in CheckResult Variable

    FunctionInfo("IfEqual", 3),          // compare a=b, jump if condition met
    FunctionInfo("IfGreater", 3),        // compare a>b, jump if condition met
    FunctionInfo("IfGreaterOrEqual", 3), // compare a>=b, jump if condition met
    FunctionInfo("IfLower", 3),          // compare a<b, jump if condition met
    FunctionInfo("IfLowerOrEqual", 3),   // compare a<=b, jump if condition met
    FunctionInfo("IfNotEqual", 3),       // compare a!=b, jump if condition met
    FunctionInfo("else", 0),             // The else for an if statement
    FunctionInfo("endif", 0),            // The end if

    FunctionInfo("WEqual", 3),          // compare a=b, loop if condition met
    FunctionInfo("WGreater", 3),        // compare a>b, loop if condition met
    FunctionInfo("WGreaterOrEqual", 3), // compare a>=b, loop if condition met
    FunctionInfo("WLower", 3),          // compare a<b, loop if condition met
    FunctionInfo("WLowerOrEqual", 3),   // compare a<=b, loop if condition met
    FunctionInfo("WNotEqual", 3),       // compare a!=b, loop if condition met
    FunctionInfo("loop", 0),            // While Loop marker

    FunctionInfo("ForEachActive", 3), // foreach loop, iterates through object group lists only if they
                                      // are active and interaction is true
    FunctionInfo("ForEachAll", 3),    // foreach loop, iterates through objects matching type
    FunctionInfo("next", 0),          // foreach loop, next marker

    FunctionInfo("switch", 2),    // Switch Statement
    FunctionInfo("break", 0),     // break
    FunctionInfo("endswitch", 0), // endswitch

    // Math Functions
    FunctionInfo("Rand", 2),
    FunctionInfo("Sin", 2),
    FunctionInfo("Cos", 2),
    FunctionInfo("Sin256", 2),
    FunctionInfo("Cos256", 2),
    FunctionInfo("ATan2", 3),
    FunctionInfo("Interpolate", 4),
    FunctionInfo("InterpolateXY", 7),

    // Graphics Functions
    FunctionInfo("LoadSpriteSheet", 1),
    FunctionInfo("RemoveSpriteSheet", 1),
    FunctionInfo("DrawSprite", 1),
    FunctionInfo("DrawSpriteXY", 3),
    FunctionInfo("DrawSpriteScreenXY", 3),
    FunctionInfo("DrawTintRect", 4),
    FunctionInfo("DrawNumbers", 7),
    FunctionInfo("DrawActName", 7),
    FunctionInfo("DrawMenu", 3),
    FunctionInfo("SpriteFrame", 6),
    FunctionInfo("EditFrame", 7),
    FunctionInfo("LoadPalette", 5),
    FunctionInfo("RotatePalette", 4),
    FunctionInfo("SetScreenFade", 4),
    FunctionInfo("SetActivePalette", 3),
    FunctionInfo("SetPaletteFade", 6),
    FunctionInfo("SetPaletteEntry", 3),
    FunctionInfo("GetPaletteEntry", 3),
    FunctionInfo("CopyPalette", 5),
    FunctionInfo("ClearScreen", 1),
    FunctionInfo("DrawSpriteFX", 4),
    FunctionInfo("DrawSpriteScreenFX", 4),

    // More Useful Stuff
    FunctionInfo("LoadAnimation", 1),
    FunctionInfo("SetupMenu", 4),
    FunctionInfo("AddMenuEntry", 3),
    FunctionInfo("EditMenuEntry", 4),
    FunctionInfo("LoadStage", 0),
    FunctionInfo("DrawRect", 8),
    FunctionInfo("ResetObjectEntity", 5),
    FunctionInfo("BoxCollisionTest", 11),
    FunctionInfo("CreateTempObject", 4),

    // Player and Animation Functions
    FunctionInfo("ProcessObjectMovement", 0),
    FunctionInfo("ProcessObjectControl", 0),
    FunctionInfo("ProcessAnimation", 0),
    FunctionInfo("DrawObjectAnimation", 0),

    // Music
    FunctionInfo("SetMusicTrack", 3),
    FunctionInfo("PlayMusic", 1),
    FunctionInfo("StopMusic", 0),
    FunctionInfo("PauseMusic", 0),
    FunctionInfo("ResumeMusic", 0),
    FunctionInfo("SwapMusicTrack", 4),

    // Sound FX
    FunctionInfo("PlaySfx", 2),
    FunctionInfo("StopSfx", 1),
    FunctionInfo("SetSfxAttributes", 3),

    // More Collision Stuff
    FunctionInfo("ObjectTileCollision", 4),
    FunctionInfo("ObjectTileGrip", 4),

    // Bitwise Not
    FunctionInfo("Not", 1),

    // 3D Stuff
    FunctionInfo("Draw3DScene", 0),
    FunctionInfo("SetIdentityMatrix", 1),
    FunctionInfo("MatrixMultiply", 2),
    FunctionInfo("MatrixTranslateXYZ", 4),
    FunctionInfo("MatrixScaleXYZ", 4),
    FunctionInfo("MatrixRotateX", 2),
    FunctionInfo("MatrixRotateY", 2),
    FunctionInfo("MatrixRotateZ", 2),
    FunctionInfo("MatrixRotateXYZ", 4),
    FunctionInfo("MatrixInverse", 1),
    FunctionInfo("TransformVertices", 3),

    FunctionInfo("CallFunction", 1),
    FunctionInfo("return", 0),

    FunctionInfo("SetLayerDeformation", 6),
    FunctionInfo("CheckTouchRect", 4),
    FunctionInfo("GetTileLayerEntry", 4),
    FunctionInfo("SetTileLayerEntry", 4),

    FunctionInfo("GetBit", 3),
    FunctionInfo("SetBit", 3),

    FunctionInfo("ClearDrawList", 1),
    FunctionInfo("AddDrawListEntityRef", 2),
    FunctionInfo("GetDrawListEntityRef", 3),
    FunctionInfo("SetDrawListEntityRef", 3),

    FunctionInfo("Get16x16TileInfo", 4),
    FunctionInfo("Set16x16TileInfo", 4),
    FunctionInfo("Copy16x16Tile", 2),
    FunctionInfo("GetAnimationByName", 2),
    FunctionInfo("ReadSaveRAM", 0),
    FunctionInfo("WriteSaveRAM", 0),

    FunctionInfo("LoadFontFile", 1),
    FunctionInfo("LoadTextFile", 3),
    FunctionInfo("GetTextInfo", 5),
    FunctionInfo("DrawText", 7),
    FunctionInfo("GetVersionNumber", 2),

    FunctionInfo("GetTableValue", 3),
    FunctionInfo("SetTableValue", 3),

    FunctionInfo("CheckCurrentStageFolder", 1),
    FunctionInfo("Abs", 1),

    FunctionInfo("CallNativeFunction", 1),
    FunctionInfo("CallNativeFunction2", 3),
    FunctionInfo("CallNativeFunction4", 5),

    FunctionInfo("SetObjectRange", 1),
    FunctionInfo("Print", 3),

    // EDITOR-ONLY
    FunctionInfo("AddEditorVariable", 1),
    FunctionInfo("SetActiveVariable", 1),
    FunctionInfo("AddEnumVariable", 2),
    FunctionInfo("SetVariableAlias", 2),
    FunctionInfo("DrawLine", 7),
    FunctionInfo("DrawArrow", 7),
    FunctionInfo("DrawRectWorld", 8),
    FunctionInfo("DrawRectOutline", 8),
    FunctionInfo("GetObjectType", 2),
};
Compilerv4::FunctionInfo Compilerv4::functionList_rev02[] = {

    FunctionInfo("End", 0),      // End of Script
    FunctionInfo("Equal", 2),    // Equal
    FunctionInfo("Add", 2),      // Add
    FunctionInfo("Sub", 2),      // Subtract
    FunctionInfo("Inc", 1),      // Increment
    FunctionInfo("Dec", 1),      // Decrement
    FunctionInfo("Mul", 2),      // Multiply
    FunctionInfo("Div", 2),      // Divide
    FunctionInfo("ShR", 2),      // Bit Shift Right
    FunctionInfo("ShL", 2),      // Bit Shift Left
    FunctionInfo("And", 2),      // Bitwise And
    FunctionInfo("Or", 2),       // Bitwise Or
    FunctionInfo("Xor", 2),      // Bitwise Xor
    FunctionInfo("Mod", 2),      // Mod
    FunctionInfo("FlipSign", 1), // Flips the Sign of the value

    FunctionInfo("CheckEqual", 2),    // compare a=b, return result in CheckResult Variable
    FunctionInfo("CheckGreater", 2),  // compare a>b, return result in CheckResult Variable
    FunctionInfo("CheckLower", 2),    // compare a<b, return result in CheckResult Variable
    FunctionInfo("CheckNotEqual", 2), // compare a!=b, return result in CheckResult Variable

    FunctionInfo("IfEqual", 3),          // compare a=b, jump if condition met
    FunctionInfo("IfGreater", 3),        // compare a>b, jump if condition met
    FunctionInfo("IfGreaterOrEqual", 3), // compare a>=b, jump if condition met
    FunctionInfo("IfLower", 3),          // compare a<b, jump if condition met
    FunctionInfo("IfLowerOrEqual", 3),   // compare a<=b, jump if condition met
    FunctionInfo("IfNotEqual", 3),       // compare a!=b, jump if condition met
    FunctionInfo("else", 0),             // The else for an if statement
    FunctionInfo("endif", 0),            // The end if

    FunctionInfo("WEqual", 3),          // compare a=b, loop if condition met
    FunctionInfo("WGreater", 3),        // compare a>b, loop if condition met
    FunctionInfo("WGreaterOrEqual", 3), // compare a>=b, loop if condition met
    FunctionInfo("WLower", 3),          // compare a<b, loop if condition met
    FunctionInfo("WLowerOrEqual", 3),   // compare a<=b, loop if condition met
    FunctionInfo("WNotEqual", 3),       // compare a!=b, loop if condition met
    FunctionInfo("loop", 0),            // While Loop marker

    FunctionInfo("ForEachActive", 3), // foreach loop, iterates through object group lists only if they
                                      // are active and interaction is true
    FunctionInfo("ForEachAll", 3),    // foreach loop, iterates through objects matching type
    FunctionInfo("next", 0),          // foreach loop, next marker

    FunctionInfo("switch", 2),    // Switch Statement
    FunctionInfo("break", 0),     // break
    FunctionInfo("endswitch", 0), // endswitch

    // Math Functions
    FunctionInfo("Rand", 2),
    FunctionInfo("Sin", 2),
    FunctionInfo("Cos", 2),
    FunctionInfo("Sin256", 2),
    FunctionInfo("Cos256", 2),
    FunctionInfo("ATan2", 3),
    FunctionInfo("Interpolate", 4),
    FunctionInfo("InterpolateXY", 7),

    // Graphics Functions
    FunctionInfo("LoadSpriteSheet", 1),
    FunctionInfo("RemoveSpriteSheet", 1),
    FunctionInfo("DrawSprite", 1),
    FunctionInfo("DrawSpriteXY", 3),
    FunctionInfo("DrawSpriteScreenXY", 3),
    FunctionInfo("DrawTintRect", 4),
    FunctionInfo("DrawNumbers", 7),
    FunctionInfo("DrawActName", 7),
    FunctionInfo("DrawMenu", 3),
    FunctionInfo("SpriteFrame", 6),
    FunctionInfo("EditFrame", 7),
    FunctionInfo("LoadPalette", 5),
    FunctionInfo("RotatePalette", 4),
    FunctionInfo("SetScreenFade", 4),
    FunctionInfo("SetActivePalette", 3),
    FunctionInfo("SetPaletteFade", 6),
    FunctionInfo("SetPaletteEntry", 3),
    FunctionInfo("GetPaletteEntry", 3),
    FunctionInfo("CopyPalette", 5),
    FunctionInfo("ClearScreen", 1),
    FunctionInfo("DrawSpriteFX", 4),
    FunctionInfo("DrawSpriteScreenFX", 4),

    // More Useful Stuff
    FunctionInfo("LoadAnimation", 1),
    FunctionInfo("SetupMenu", 4),
    FunctionInfo("AddMenuEntry", 3),
    FunctionInfo("EditMenuEntry", 4),
    FunctionInfo("LoadStage", 0),
    FunctionInfo("DrawRect", 8),
    FunctionInfo("ResetObjectEntity", 5),
    FunctionInfo("BoxCollisionTest", 11),
    FunctionInfo("CreateTempObject", 4),

    // Player and Animation Functions
    FunctionInfo("ProcessObjectMovement", 0),
    FunctionInfo("ProcessObjectControl", 0),
    FunctionInfo("ProcessAnimation", 0),
    FunctionInfo("DrawObjectAnimation", 0),

    // Music
    FunctionInfo("SetMusicTrack", 3),
    FunctionInfo("PlayMusic", 1),
    FunctionInfo("StopMusic", 0),
    FunctionInfo("PauseMusic", 0),
    FunctionInfo("ResumeMusic", 0),
    FunctionInfo("SwapMusicTrack", 4),

    // Sound FX
    FunctionInfo("PlaySfx", 2),
    FunctionInfo("StopSfx", 1),
    FunctionInfo("SetSfxAttributes", 3),

    // More Collision Stuff
    FunctionInfo("ObjectTileCollision", 4),
    FunctionInfo("ObjectTileGrip", 4),

    // Bitwise Not
    FunctionInfo("Not", 1),

    // 3D Stuff
    FunctionInfo("Draw3DScene", 0),
    FunctionInfo("SetIdentityMatrix", 1),
    FunctionInfo("MatrixMultiply", 2),
    FunctionInfo("MatrixTranslateXYZ", 4),
    FunctionInfo("MatrixScaleXYZ", 4),
    FunctionInfo("MatrixRotateX", 2),
    FunctionInfo("MatrixRotateY", 2),
    FunctionInfo("MatrixRotateZ", 2),
    FunctionInfo("MatrixRotateXYZ", 4),
    FunctionInfo("MatrixInverse", 1),
    FunctionInfo("TransformVertices", 3),

    FunctionInfo("CallFunction", 1),
    FunctionInfo("return", 0),

    FunctionInfo("SetLayerDeformation", 6),
    FunctionInfo("CheckTouchRect", 4),
    FunctionInfo("GetTileLayerEntry", 4),
    FunctionInfo("SetTileLayerEntry", 4),

    FunctionInfo("GetBit", 3),
    FunctionInfo("SetBit", 3),

    FunctionInfo("ClearDrawList", 1),
    FunctionInfo("AddDrawListEntityRef", 2),
    FunctionInfo("GetDrawListEntityRef", 3),
    FunctionInfo("SetDrawListEntityRef", 3),

    FunctionInfo("Get16x16TileInfo", 4),
    FunctionInfo("Set16x16TileInfo", 4),
    FunctionInfo("Copy16x16Tile", 2),
    FunctionInfo("GetAnimationByName", 2),
    FunctionInfo("ReadSaveRAM", 0),
    FunctionInfo("WriteSaveRAM", 0),

    FunctionInfo("LoadTextFile", 2),
    FunctionInfo("GetTextInfo", 5),
    FunctionInfo("GetVersionNumber", 2),

    FunctionInfo("GetTableValue", 3),
    FunctionInfo("SetTableValue", 3),

    FunctionInfo("CheckCurrentStageFolder", 1),
    FunctionInfo("Abs", 1),

    FunctionInfo("CallNativeFunction", 1),
    FunctionInfo("CallNativeFunction2", 3),
    FunctionInfo("CallNativeFunction4", 5),

    FunctionInfo("SetObjectRange", 1),
    FunctionInfo("GetObjectValue", 3),
    FunctionInfo("SetObjectValue", 3),
    FunctionInfo("CopyObject", 3),
    FunctionInfo("Print", 3),

    // EDITOR-ONLY
    FunctionInfo("AddEditorVariable", 1),
    FunctionInfo("SetActiveVariable", 1),
    FunctionInfo("AddEnumVariable", 2),
    FunctionInfo("SetVariableAlias", 2),
    FunctionInfo("DrawLine", 7),
    FunctionInfo("DrawArrow", 7),
    FunctionInfo("DrawRectWorld", 8),
    FunctionInfo("DrawRectOutline", 8),
    FunctionInfo("GetObjectType", 2),
};
Compilerv4::FunctionInfo Compilerv4::functionList_rev03[] = {

    FunctionInfo("End", 0),      // End of Script
    FunctionInfo("Equal", 2),    // Equal
    FunctionInfo("Add", 2),      // Add
    FunctionInfo("Sub", 2),      // Subtract
    FunctionInfo("Inc", 1),      // Increment
    FunctionInfo("Dec", 1),      // Decrement
    FunctionInfo("Mul", 2),      // Multiply
    FunctionInfo("Div", 2),      // Divide
    FunctionInfo("ShR", 2),      // Bit Shift Right
    FunctionInfo("ShL", 2),      // Bit Shift Left
    FunctionInfo("And", 2),      // Bitwise And
    FunctionInfo("Or", 2),       // Bitwise Or
    FunctionInfo("Xor", 2),      // Bitwise Xor
    FunctionInfo("Mod", 2),      // Mod
    FunctionInfo("FlipSign", 1), // Flips the Sign of the value

    FunctionInfo("CheckEqual", 2),    // compare a=b, return result in CheckResult Variable
    FunctionInfo("CheckGreater", 2),  // compare a>b, return result in CheckResult Variable
    FunctionInfo("CheckLower", 2),    // compare a<b, return result in CheckResult Variable
    FunctionInfo("CheckNotEqual", 2), // compare a!=b, return result in CheckResult Variable

    FunctionInfo("IfEqual", 3),          // compare a=b, jump if condition met
    FunctionInfo("IfGreater", 3),        // compare a>b, jump if condition met
    FunctionInfo("IfGreaterOrEqual", 3), // compare a>=b, jump if condition met
    FunctionInfo("IfLower", 3),          // compare a<b, jump if condition met
    FunctionInfo("IfLowerOrEqual", 3),   // compare a<=b, jump if condition met
    FunctionInfo("IfNotEqual", 3),       // compare a!=b, jump if condition met
    FunctionInfo("else", 0),             // The else for an if statement
    FunctionInfo("endif", 0),            // The end if

    FunctionInfo("WEqual", 3),          // compare a=b, loop if condition met
    FunctionInfo("WGreater", 3),        // compare a>b, loop if condition met
    FunctionInfo("WGreaterOrEqual", 3), // compare a>=b, loop if condition met
    FunctionInfo("WLower", 3),          // compare a<b, loop if condition met
    FunctionInfo("WLowerOrEqual", 3),   // compare a<=b, loop if condition met
    FunctionInfo("WNotEqual", 3),       // compare a!=b, loop if condition met
    FunctionInfo("loop", 0),            // While Loop marker

    FunctionInfo("ForEachActive", 3), // foreach loop, iterates through object group lists only if they
                                      // are active and interaction is true
    FunctionInfo("ForEachAll", 3),    // foreach loop, iterates through objects matching type
    FunctionInfo("next", 0),          // foreach loop, next marker

    FunctionInfo("switch", 2),    // Switch Statement
    FunctionInfo("break", 0),     // break
    FunctionInfo("endswitch", 0), // endswitch

    // Math Functions
    FunctionInfo("Rand", 2),
    FunctionInfo("Sin", 2),
    FunctionInfo("Cos", 2),
    FunctionInfo("Sin256", 2),
    FunctionInfo("Cos256", 2),
    FunctionInfo("ATan2", 3),
    FunctionInfo("Interpolate", 4),
    FunctionInfo("InterpolateXY", 7),

    // Graphics Functions
    FunctionInfo("LoadSpriteSheet", 1),
    FunctionInfo("RemoveSpriteSheet", 1),
    FunctionInfo("DrawSprite", 1),
    FunctionInfo("DrawSpriteXY", 3),
    FunctionInfo("DrawSpriteScreenXY", 3),
    FunctionInfo("DrawTintRect", 4),
    FunctionInfo("DrawNumbers", 7),
    FunctionInfo("DrawActName", 7),
    FunctionInfo("DrawMenu", 3),
    FunctionInfo("SpriteFrame", 6),
    FunctionInfo("EditFrame", 7),
    FunctionInfo("LoadPalette", 5),
    FunctionInfo("RotatePalette", 4),
    FunctionInfo("SetScreenFade", 4),
    FunctionInfo("SetActivePalette", 3),
    FunctionInfo("SetPaletteFade", 6),
    FunctionInfo("SetPaletteEntry", 3),
    FunctionInfo("GetPaletteEntry", 3),
    FunctionInfo("CopyPalette", 5),
    FunctionInfo("ClearScreen", 1),
    FunctionInfo("DrawSpriteFX", 4),
    FunctionInfo("DrawSpriteScreenFX", 4),

    // More Useful Stuff
    FunctionInfo("LoadAnimation", 1),
    FunctionInfo("SetupMenu", 4),
    FunctionInfo("AddMenuEntry", 3),
    FunctionInfo("EditMenuEntry", 4),
    FunctionInfo("LoadStage", 0),
    FunctionInfo("DrawRect", 8),
    FunctionInfo("ResetObjectEntity", 5),
    FunctionInfo("BoxCollisionTest", 11),
    FunctionInfo("CreateTempObject", 4),

    // Player and Animation Functions
    FunctionInfo("ProcessObjectMovement", 0),
    FunctionInfo("ProcessObjectControl", 0),
    FunctionInfo("ProcessAnimation", 0),
    FunctionInfo("DrawObjectAnimation", 0),

    // Music
    FunctionInfo("SetMusicTrack", 3),
    FunctionInfo("PlayMusic", 1),
    FunctionInfo("StopMusic", 0),
    FunctionInfo("PauseMusic", 0),
    FunctionInfo("ResumeMusic", 0),
    FunctionInfo("SwapMusicTrack", 4),

    // Sound FX
    FunctionInfo("PlaySfx", 2),
    FunctionInfo("StopSfx", 1),
    FunctionInfo("SetSfxAttributes", 3),

    // More Collision Stuff
    FunctionInfo("ObjectTileCollision", 4),
    FunctionInfo("ObjectTileGrip", 4),

    // Bitwise Not
    FunctionInfo("Not", 1),

    // 3D Stuff
    FunctionInfo("Draw3DScene", 0),
    FunctionInfo("SetIdentityMatrix", 1),
    FunctionInfo("MatrixMultiply", 2),
    FunctionInfo("MatrixTranslateXYZ", 4),
    FunctionInfo("MatrixScaleXYZ", 4),
    FunctionInfo("MatrixRotateX", 2),
    FunctionInfo("MatrixRotateY", 2),
    FunctionInfo("MatrixRotateZ", 2),
    FunctionInfo("MatrixRotateXYZ", 4),
    FunctionInfo("MatrixInverse", 1),
    FunctionInfo("TransformVertices", 3),

    FunctionInfo("CallFunction", 1),
    FunctionInfo("return", 0),

    FunctionInfo("SetLayerDeformation", 6),
    FunctionInfo("CheckTouchRect", 4),
    FunctionInfo("GetTileLayerEntry", 4),
    FunctionInfo("SetTileLayerEntry", 4),

    FunctionInfo("GetBit", 3),
    FunctionInfo("SetBit", 3),

    FunctionInfo("ClearDrawList", 1),
    FunctionInfo("AddDrawListEntityRef", 2),
    FunctionInfo("GetDrawListEntityRef", 3),
    FunctionInfo("SetDrawListEntityRef", 3),

    FunctionInfo("Get16x16TileInfo", 4),
    FunctionInfo("Set16x16TileInfo", 4),
    FunctionInfo("Copy16x16Tile", 2),
    FunctionInfo("GetAnimationByName", 2),
    FunctionInfo("ReadSaveRAM", 0),
    FunctionInfo("WriteSaveRAM", 0),

    FunctionInfo("LoadTextFile", 2),
    FunctionInfo("GetTextInfo", 5),
    FunctionInfo("GetVersionNumber", 2),

    FunctionInfo("GetTableValue", 3),
    FunctionInfo("SetTableValue", 3),

    FunctionInfo("CheckCurrentStageFolder", 1),
    FunctionInfo("Abs", 1),

    FunctionInfo("CallNativeFunction", 1),
    FunctionInfo("CallNativeFunction2", 3),
    FunctionInfo("CallNativeFunction4", 5),

    FunctionInfo("SetObjectRange", 1),
    FunctionInfo("GetObjectValue", 3),
    FunctionInfo("SetObjectValue", 3),
    FunctionInfo("CopyObject", 3),
    FunctionInfo("Print", 3),

    // Extras
    FunctionInfo("CheckCameraProximity", 4),
    FunctionInfo("SetScreenCount", 1),
    FunctionInfo("SetScreenVertices", 5),
    FunctionInfo("GetInputDeviceID", 2),
    FunctionInfo("GetFilteredInputDeviceID", 4),
    FunctionInfo("GetInputDeviceType", 2),
    FunctionInfo("IsInputDeviceAssigned", 1),
    FunctionInfo("AssignInputSlotToDevice", 2),
    FunctionInfo("IsInputSlotAssigned", 1),
    FunctionInfo("ResetInputSlotAssignments", 0),

    // EDITOR-ONLY
    FunctionInfo("AddEditorVariable", 1),
    FunctionInfo("SetActiveVariable", 1),
    FunctionInfo("AddEnumVariable", 2),
    FunctionInfo("SetVariableAlias", 2),
    FunctionInfo("DrawLine", 7),
    FunctionInfo("DrawArrow", 7),
    FunctionInfo("DrawRectWorld", 8),
    FunctionInfo("DrawRectOutline", 8),
    FunctionInfo("GetObjectType", 2),
};

// clang-format off
Compilerv4::ScriptVariableInfo Compilerv4::scriptValueList[SCRIPT_VAR_COUNT_v4] = {
    // ORIGINAL ALIASES
    // These are in the official v4
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "true", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "false", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FX_SCALE", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FX_ROTATE", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FX_ROTOZOOM", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FX_INK", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PRESENTATION_STAGE", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "REGULAR_STAGE", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "BONUS_STAGE", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "SPECIAL_STAGE", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "MENU_1", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "MENU_2", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "C_TOUCH", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "C_SOLID", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "C_SOLID2", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "C_PLATFORM", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "C_BOX", "65536"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "MAT_WORLD", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "MAT_VIEW", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "MAT_TEMP", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FX_FLIP", "5"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACING_LEFT", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACING_RIGHT", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "STAGE_2P_MODE", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "STAGE_FROZEN", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "STAGE_PAUSED", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "STAGE_RUNNING", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "RESET_GAME", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "STANDARD", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "MOBILE", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "DEVICE_XBOX", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "DEVICE_PSN", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "DEVICE_IOS", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "DEVICE_ANDROID", "5"),

    // Decomp custom aliases
    // Aliases that do not exist in the official version in any form
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FLIP_NONE", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FLIP_X", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FLIP_Y", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FLIP_XY", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ENGINE_DEVMENU", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ENGINE_MAINGAME", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ENGINE_INITDEVMENU", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ENGINE_WAIT", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ENGINE_SCRIPTERROR", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ENGINE_INITPAUSE", "5"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ENGINE_EXITPAUSE", "6"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ENGINE_ENDGAME", "7"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ENGINE_RESETGAME", "8"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "INK_NONE", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "INK_BLEND", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "INK_ALPHA", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "INK_ADD", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "INK_SUB", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CSIDE_FLOOR", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CSIDE_LWALL", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CSIDE_RWALL", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CSIDE_ROOF", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CMODE_FLOOR", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CMODE_LWALL", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CMODE_ROOF", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CMODE_RWALL", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "COL_NONE", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "COL_TOP", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "COL_LEFT", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "COL_RIGHT", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "COL_BOTTOM", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PATH_A", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PATH_B", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "GRAVITY_GROUND", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "GRAVITY_AIR", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACE_TEXTURED_3D", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACE_TEXTURED_2D", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACE_COLOURED_3D", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACE_COLOURED_2D", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACE_FADED", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACE_TEXTURED_C", "5"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACE_TEXTURED_C_BLEND", "6"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "FACE_SPRITE_3D", "7"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PRIORITY_ACTIVE_BOUNDS", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PRIORITY_ACTIVE", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PRIORITY_ACTIVE_PAUSED", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PRIORITY_XBOUNDS", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PRIORITY_XBOUNDS_DESTROY", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PRIORITY_INACTIVE", "5"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PRIORITY_BOUNDS_SMALL", "6"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "PRIORITY_ACTIVE_SMALL", "7"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CONTROLMODE_NONE", "-1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CONTROLMODE_NORMAL", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CAMERASTYLE_FOLLOW", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CAMERASTYLE_EXTENDED", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CAMERASTYLE_EXTENDED_OFFSET_L", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CAMERASTYLE_EXTENDED_OFFSET_R", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "CAMERASTYLE_HLOCKED", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILEINFO_INDEX", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILEINFO_DIRECTION", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILEINFO_VISUALPLANE", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILEINFO_SOLIDITYA", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILEINFO_SOLIDITYB", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILEINFO_FLAGSA", "5"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILEINFO_ANGLEA", "6"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILEINFO_FLAGSB", "7"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILEINFO_ANGLEB", "8"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TEXTINFO_TEXTDATA", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TEXTINFO_TEXTSIZE", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TEXTINFO_ROWCOUNT", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILELAYER_NOSCROLL", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILELAYER_HSCROLL", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILELAYER_VSCROLL", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILELAYER_3DFLOOR", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "TILELAYER_3DSKY", "4"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "GROUP_ALL", "0"),

    // EDITOR-ONLY
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "EDIT_VAR_PROPVAL", "-1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ALIAS_VAR_PROPVAL", "0"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ALIAS_VAR_VAL0", "1"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ALIAS_VAR_VAL1", "2"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ALIAS_VAR_VAL2", "3"),
    ScriptVariableInfo(VAR_ALIAS, ACCESS_PUBLIC, "ALIAS_VAR_VAL3", "4"),
};
// clang-format on

const char scriptEvaluationTokens[][0x4] = { "=",   "+=",  "-=", "++", "--", "*=", "/=",
                                             ">>=", "<<=", "&=", "|=", "^=", "%=", "==",
                                             ">",   ">=",  "<",  "<=", "!=" };

enum ScriptReadModes {
    READMODE_NORMAL      = 0,
    READMODE_STRING      = 1,
    READMODE_COMMENTLINE = 2,
    READMODE_ENDLINE     = 3,
    READMODE_EOF         = 4
};
enum ScriptParseModes {
    PARSEMODE_SCOPELESS    = 0,
    PARSEMODE_PLATFORMSKIP = 1,
    PARSEMODE_FUNCTION     = 2,
    PARSEMODE_SWITCHREAD   = 3,
    PARSEMODE_TABLEREAD    = 4,
    PARSEMODE_ERROR        = 0xFF
};

enum ScriptVarTypes { SCRIPTVAR_VAR = 1, SCRIPTVAR_INTCONST = 2, SCRIPTVAR_STRCONST = 3 };
enum ScriptVarArrTypes {
    VARARR_NONE        = 0,
    VARARR_ARRAY       = 1,
    VARARR_ENTNOPLUS1  = 2,
    VARARR_ENTNOMINUS1 = 3
};

enum ScrVar {
    VAR_TEMP0,
    VAR_TEMP1,
    VAR_TEMP2,
    VAR_TEMP3,
    VAR_TEMP4,
    VAR_TEMP5,
    VAR_TEMP6,
    VAR_TEMP7,
    VAR_CHECKRESULT,
    VAR_ARRAYPOS0,
    VAR_ARRAYPOS1,
    VAR_ARRAYPOS2,
    VAR_ARRAYPOS3,
    VAR_ARRAYPOS4,
    VAR_ARRAYPOS5,
    VAR_ARRAYPOS6,
    VAR_ARRAYPOS7,
    VAR_GLOBAL,
    VAR_LOCAL,
    VAR_OBJECTENTITYPOS,
    VAR_OBJECTGROUPID,
    VAR_OBJECTTYPE,
    VAR_OBJECTPROPERTYVALUE,
    VAR_OBJECTXPOS,
    VAR_OBJECTYPOS,
    VAR_OBJECTIXPOS,
    VAR_OBJECTIYPOS,
    VAR_OBJECTXVEL,
    VAR_OBJECTYVEL,
    VAR_OBJECTSPEED,
    VAR_OBJECTSTATE,
    VAR_OBJECTROTATION,
    VAR_OBJECTSCALE,
    VAR_OBJECTPRIORITY,
    VAR_OBJECTDRAWORDER,
    VAR_OBJECTDIRECTION,
    VAR_OBJECTINKEFFECT,
    VAR_OBJECTALPHA,
    VAR_OBJECTFRAME,
    VAR_OBJECTANIMATION,
    VAR_OBJECTPREVANIMATION,
    VAR_OBJECTANIMATIONSPEED,
    VAR_OBJECTANIMATIONTIMER,
    VAR_OBJECTANGLE,
    VAR_OBJECTLOOKPOSX,
    VAR_OBJECTLOOKPOSY,
    VAR_OBJECTCOLLISIONMODE,
    VAR_OBJECTCOLLISIONPLANE,
    VAR_OBJECTCONTROLMODE,
    VAR_OBJECTCONTROLLOCK,
    VAR_OBJECTPUSHING,
    VAR_OBJECTVISIBLE,
    VAR_OBJECTTILECOLLISIONS,
    VAR_OBJECTINTERACTION,
    VAR_OBJECTGRAVITY,
    VAR_OBJECTUP,
    VAR_OBJECTDOWN,
    VAR_OBJECTLEFT,
    VAR_OBJECTRIGHT,
    VAR_OBJECTJUMPPRESS,
    VAR_OBJECTJUMPHOLD,
    VAR_OBJECTSCROLLTRACKING,
    VAR_OBJECTFLOORSENSORL,
    VAR_OBJECTFLOORSENSORC,
    VAR_OBJECTFLOORSENSORR,
    VAR_OBJECTFLOORSENSORLC,
    VAR_OBJECTFLOORSENSORRC,
    VAR_OBJECTCOLLISIONLEFT,
    VAR_OBJECTCOLLISIONTOP,
    VAR_OBJECTCOLLISIONRIGHT,
    VAR_OBJECTCOLLISIONBOTTOM,
    VAR_OBJECTOUTOFBOUNDS,
    VAR_OBJECTSPRITESHEET,
    VAR_OBJECTVALUE0,
    VAR_OBJECTVALUE1,
    VAR_OBJECTVALUE2,
    VAR_OBJECTVALUE3,
    VAR_OBJECTVALUE4,
    VAR_OBJECTVALUE5,
    VAR_OBJECTVALUE6,
    VAR_OBJECTVALUE7,
    VAR_OBJECTVALUE8,
    VAR_OBJECTVALUE9,
    VAR_OBJECTVALUE10,
    VAR_OBJECTVALUE11,
    VAR_OBJECTVALUE12,
    VAR_OBJECTVALUE13,
    VAR_OBJECTVALUE14,
    VAR_OBJECTVALUE15,
    VAR_OBJECTVALUE16,
    VAR_OBJECTVALUE17,
    VAR_OBJECTVALUE18,
    VAR_OBJECTVALUE19,
    VAR_OBJECTVALUE20,
    VAR_OBJECTVALUE21,
    VAR_OBJECTVALUE22,
    VAR_OBJECTVALUE23,
    VAR_OBJECTVALUE24,
    VAR_OBJECTVALUE25,
    VAR_OBJECTVALUE26,
    VAR_OBJECTVALUE27,
    VAR_OBJECTVALUE28,
    VAR_OBJECTVALUE29,
    VAR_OBJECTVALUE30,
    VAR_OBJECTVALUE31,
    VAR_OBJECTVALUE32,
    VAR_OBJECTVALUE33,
    VAR_OBJECTVALUE34,
    VAR_OBJECTVALUE35,
    VAR_OBJECTVALUE36,
    VAR_OBJECTVALUE37,
    VAR_OBJECTVALUE38,
    VAR_OBJECTVALUE39,
    VAR_OBJECTVALUE40,
    VAR_OBJECTVALUE41,
    VAR_OBJECTVALUE42,
    VAR_OBJECTVALUE43,
    VAR_OBJECTVALUE44,
    VAR_OBJECTVALUE45,
    VAR_OBJECTVALUE46,
    VAR_OBJECTVALUE47,
    VAR_STAGESTATE,
    VAR_STAGEACTIVELIST,
    VAR_STAGELISTPOS,
    VAR_STAGETIMEENABLED,
    VAR_STAGEMILLISECONDS,
    VAR_STAGESECONDS,
    VAR_STAGEMINUTES,
    VAR_STAGEACTNUM,
    VAR_STAGEPAUSEENABLED,
    VAR_STAGELISTSIZE,
    VAR_STAGENEWXBOUNDARY1,
    VAR_STAGENEWXBOUNDARY2,
    VAR_STAGENEWYBOUNDARY1,
    VAR_STAGENEWYBOUNDARY2,
    VAR_STAGECURXBOUNDARY1,
    VAR_STAGECURXBOUNDARY2,
    VAR_STAGECURYBOUNDARY1,
    VAR_STAGECURYBOUNDARY2,
    VAR_STAGEDEFORMATIONDATA0,
    VAR_STAGEDEFORMATIONDATA1,
    VAR_STAGEDEFORMATIONDATA2,
    VAR_STAGEDEFORMATIONDATA3,
    VAR_STAGEWATERLEVEL,
    VAR_STAGEACTIVELAYER,
    VAR_STAGEMIDPOINT,
    VAR_STAGEPLAYERLISTPOS,
    VAR_STAGEDEBUGMODE,
    VAR_STAGEENTITYPOS,
    VAR_SCREENCAMERAENABLED,
    VAR_SCREENCAMERATARGET,
    VAR_SCREENCAMERASTYLE,
    VAR_SCREENCAMERAX,
    VAR_SCREENCAMERAY,
    VAR_SCREENDRAWLISTSIZE,
    VAR_SCREENXCENTER,
    VAR_SCREENYCENTER,
    VAR_SCREENXSIZE,
    VAR_SCREENYSIZE,
    VAR_SCREENXOFFSET,
    VAR_SCREENYOFFSET,
    VAR_SCREENSHAKEX,
    VAR_SCREENSHAKEY,
    VAR_SCREENADJUSTCAMERAY,
    VAR_TOUCHSCREENDOWN,
    VAR_TOUCHSCREENXPOS,
    VAR_TOUCHSCREENYPOS,
    VAR_MUSICVOLUME,
    VAR_MUSICCURRENTTRACK,
    VAR_MUSICPOSITION,
    VAR_KEYDOWNUP,
    VAR_KEYDOWNDOWN,
    VAR_KEYDOWNLEFT,
    VAR_KEYDOWNRIGHT,
    VAR_KEYDOWNBUTTONA,
    VAR_KEYDOWNBUTTONB,
    VAR_KEYDOWNBUTTONC,
    VAR_KEYDOWNBUTTONX,
    VAR_KEYDOWNBUTTONY,
    VAR_KEYDOWNBUTTONZ,
    VAR_KEYDOWNBUTTONL,
    VAR_KEYDOWNBUTTONR,
    VAR_KEYDOWNSTART,
    VAR_KEYDOWNSELECT,
    VAR_KEYPRESSUP,
    VAR_KEYPRESSDOWN,
    VAR_KEYPRESSLEFT,
    VAR_KEYPRESSRIGHT,
    VAR_KEYPRESSBUTTONA,
    VAR_KEYPRESSBUTTONB,
    VAR_KEYPRESSBUTTONC,
    VAR_KEYPRESSBUTTONX,
    VAR_KEYPRESSBUTTONY,
    VAR_KEYPRESSBUTTONZ,
    VAR_KEYPRESSBUTTONL,
    VAR_KEYPRESSBUTTONR,
    VAR_KEYPRESSSTART,
    VAR_KEYPRESSSELECT,
    VAR_MENU1SELECTION,
    VAR_MENU2SELECTION,
    VAR_TILELAYERXSIZE,
    VAR_TILELAYERYSIZE,
    VAR_TILELAYERTYPE,
    VAR_TILELAYERANGLE,
    VAR_TILELAYERXPOS,
    VAR_TILELAYERYPOS,
    VAR_TILELAYERZPOS,
    VAR_TILELAYERPARALLAXFACTOR,
    VAR_TILELAYERSCROLLSPEED,
    VAR_TILELAYERSCROLLPOS,
    VAR_TILELAYERDEFORMATIONOFFSET,
    VAR_TILELAYERDEFORMATIONOFFSETW,
    VAR_HPARALLAXPARALLAXFACTOR,
    VAR_HPARALLAXSCROLLSPEED,
    VAR_HPARALLAXSCROLLPOS,
    VAR_VPARALLAXPARALLAXFACTOR,
    VAR_VPARALLAXSCROLLSPEED,
    VAR_VPARALLAXSCROLLPOS,
    VAR_SCENE3DVERTEXCOUNT,
    VAR_SCENE3DFACECOUNT,
    VAR_SCENE3DPROJECTIONX,
    VAR_SCENE3DPROJECTIONY,
    VAR_SCENE3DFOGCOLOR,
    VAR_SCENE3DFOGSTRENGTH,
    VAR_VERTEXBUFFERX,
    VAR_VERTEXBUFFERY,
    VAR_VERTEXBUFFERZ,
    VAR_VERTEXBUFFERU,
    VAR_VERTEXBUFFERV,
    VAR_FACEBUFFERA,
    VAR_FACEBUFFERB,
    VAR_FACEBUFFERC,
    VAR_FACEBUFFERD,
    VAR_FACEBUFFERFLAG,
    VAR_FACEBUFFERCOLOR,
    VAR_SAVERAM,
    VAR_ENGINESTATE,
    VAR_ENGINELANGUAGE,
    VAR_ENGINEONLINEACTIVE,
    VAR_ENGINESFXVOLUME,
    VAR_ENGINEBGMVOLUME,
    VAR_ENGINETRIALMODE,
    VAR_ENGINEDEVICETYPE, // v4-style device type aka Standard/Mobile/Etc

    // Extras
    VAR_SCREENCURRENTID,
    VAR_CAMERAENABLED,
    VAR_CAMERATARGET,
    VAR_CAMERASTYLE,
    VAR_CAMERAXPOS,
    VAR_CAMERAYPOS,
    VAR_CAMERAADJUSTY,

    VAR_HAPTICSENABLED,

    // EDITOR-ONLY
    VAR_EDITORVARIABLEID,
    VAR_EDITORVARIABLEVAL,
    VAR_EDITORRETURNVAR,
    VAR_EDITORSHOWGIZMOS,
    VAR_EDITORDRAWINGOVERLAY,

    VAR_MAX_CNT
};

enum ScrFunc {
    FUNC_END,
    FUNC_EQUAL,
    FUNC_ADD,
    FUNC_SUB,
    FUNC_INC,
    FUNC_DEC,
    FUNC_MUL,
    FUNC_DIV,
    FUNC_SHR,
    FUNC_SHL,
    FUNC_AND,
    FUNC_OR,
    FUNC_XOR,
    FUNC_MOD,
    FUNC_FLIPSIGN,
    FUNC_CHECKEQUAL,
    FUNC_CHECKGREATER,
    FUNC_CHECKLOWER,
    FUNC_CHECKNOTEQUAL,
    FUNC_IFEQUAL,
    FUNC_IFGREATER,
    FUNC_IFGREATEROREQUAL,
    FUNC_IFLOWER,
    FUNC_IFLOWEROREQUAL,
    FUNC_IFNOTEQUAL,
    FUNC_ELSE,
    FUNC_ENDIF,
    FUNC_WEQUAL,
    FUNC_WGREATER,
    FUNC_WGREATEROREQUAL,
    FUNC_WLOWER,
    FUNC_WLOWEROREQUAL,
    FUNC_WNOTEQUAL,
    FUNC_LOOP,
    FUNC_FOREACHACTIVE,
    FUNC_FOREACHALL,
    FUNC_NEXT,
    FUNC_SWITCH,
    FUNC_BREAK,
    FUNC_ENDSWITCH,
    FUNC_RAND,
    FUNC_SIN,
    FUNC_COS,
    FUNC_SIN256,
    FUNC_COS256,
    FUNC_ATAN2,
    FUNC_INTERPOLATE,
    FUNC_INTERPOLATEXY,
    FUNC_LOADSPRITESHEET,
    FUNC_REMOVESPRITESHEET,
    FUNC_DRAWSPRITE,
    FUNC_DRAWSPRITEXY,
    FUNC_DRAWSPRITESCREENXY,
    FUNC_DRAWTINTRECT,
    FUNC_DRAWNUMBERS,
    FUNC_DRAWACTNAME,
    FUNC_DRAWMENU,
    FUNC_SPRITEFRAME,
    FUNC_EDITFRAME,
    FUNC_LOADPALETTE,
    FUNC_ROTATEPALETTE,
    FUNC_SETSCREENFADE,
    FUNC_SETACTIVEPALETTE,
    FUNC_SETPALETTEFADE,
    FUNC_SETPALETTEENTRY,
    FUNC_GETPALETTEENTRY,
    FUNC_COPYPALETTE,
    FUNC_CLEARSCREEN,
    FUNC_DRAWSPRITEFX,
    FUNC_DRAWSPRITESCREENFX,
    FUNC_LOADANIMATION,
    FUNC_SETUPMENU,
    FUNC_ADDMENUENTRY,
    FUNC_EDITMENUENTRY,
    FUNC_LOADSTAGE,
    FUNC_DRAWRECT,
    FUNC_RESETOBJECTENTITY,
    FUNC_BOXCOLLISIONTEST,
    FUNC_CREATETEMPOBJECT,
    FUNC_PROCESSOBJECTMOVEMENT,
    FUNC_PROCESSOBJECTCONTROL,
    FUNC_PROCESSANIMATION,
    FUNC_DRAWOBJECTANIMATION,
    FUNC_SETMUSICTRACK,
    FUNC_PLAYMUSIC,
    FUNC_STOPMUSIC,
    FUNC_PAUSEMUSIC,
    FUNC_RESUMEMUSIC,
    FUNC_SWAPMUSICTRACK,
    FUNC_PLAYSFX,
    FUNC_STOPSFX,
    FUNC_SETSFXATTRIBUTES,
    FUNC_OBJECTTILECOLLISION,
    FUNC_OBJECTTILEGRIP,
    FUNC_NOT,
    FUNC_DRAW3DSCENE,
    FUNC_SETIDENTITYMATRIX,
    FUNC_MATRIXMULTIPLY,
    FUNC_MATRIXTRANSLATEXYZ,
    FUNC_MATRIXSCALEXYZ,
    FUNC_MATRIXROTATEX,
    FUNC_MATRIXROTATEY,
    FUNC_MATRIXROTATEZ,
    FUNC_MATRIXROTATEXYZ,
    FUNC_MATRIXINVERSE,
    FUNC_TRANSFORMVERTICES,
    FUNC_CALLFUNCTION,
    FUNC_RETURN,
    FUNC_SETLAYERDEFORMATION,
    FUNC_CHECKTOUCHRECT,
    FUNC_GETTILELAYERENTRY,
    FUNC_SETTILELAYERENTRY,
    FUNC_GETBIT,
    FUNC_SETBIT,
    FUNC_CLEARDRAWLIST,
    FUNC_ADDDRAWLISTENTITYREF,
    FUNC_GETDRAWLISTENTITYREF,
    FUNC_SETDRAWLISTENTITYREF,
    FUNC_GET16X16TILEINFO,
    FUNC_SET16X16TILEINFO,
    FUNC_COPY16X16TILE,
    FUNC_GETANIMATIONBYNAME,
    FUNC_READSAVERAM,
    FUNC_WRITESAVERAM,
    FUNC_LOADTEXTFILE,
    FUNC_GETTEXTINFO,
    FUNC_GETVERSIONNUMBER,
    FUNC_GETTABLEVALUE,
    FUNC_SETTABLEVALUE,
    FUNC_CHECKCURRENTSTAGEFOLDER,
    FUNC_ABS,
    FUNC_CALLNATIVEFUNCTION,
    FUNC_CALLNATIVEFUNCTION2,
    FUNC_CALLNATIVEFUNCTION4,
    FUNC_SETOBJECTRANGE,
    FUNC_GETOBJECTVALUE,
    FUNC_SETOBJECTVALUE,
    FUNC_COPYOBJECT,
    FUNC_PRINT,

    // Extras
    FUNC_CHECKCAMERAPROXIMITY,
    FUNC_SETSCREENCOUNT,
    FUNC_SETSCREENVERTICES,
    FUNC_GETINPUTDEVICEID,
    FUNC_GETFILTEREDINPUTDEVICEID,
    FUNC_GETINPUTDEVICETYPE,
    FUNC_ISINPUTDEVICEASSIGNED,
    FUNC_ASSIGNINPUTSLOTTODEVICE,
    FUNC_ISSLOTASSIGNED,
    FUNC_RESETINPUTSLOTASSIGNMENTS,

    // EDITOR-ONLY
    FUNC_ADDEDITORVAR,
    FUNC_SETACTIVEVAR,
    FUNC_ADDENUMVAR,
    FUNC_SETVARALIAS,
    FUNC_DRAWLINE,
    FUNC_DRAWARROW,
    FUNC_DRAWRECTWORLD,
    FUNC_DRAWRECTOUTLINE,
    FUNC_GETOBJECTTYPE,

    FUNC_MAX_CNT
};

Compilerv4::Compilerv4()
{
    for (int i = 0; i < 0x200; ++i) {
        float Val    = sinf(((float)i / 256) * M_PI);
        sinVal512[i] = (signed int)(Val * 512.0);
        Val          = cosf(((float)i / 256) * M_PI);
        cosVal512[i] = (signed int)(Val * 512.0);
    }

    cosVal512[0]   = 0x200;
    cosVal512[128] = 0;
    cosVal512[256] = -0x200;
    cosVal512[384] = 0;
    sinVal512[0]   = 0;
    sinVal512[128] = 0x200;
    sinVal512[256] = 0;
    sinVal512[384] = -0x200;

    for (int i = 0; i < 0x100; i++) {
        sinVal256[i] = (sinVal512[i * 2] >> 1);
        cosVal256[i] = (cosVal512[i * 2] >> 1);
    }

    for (int Y = 0; Y < 0x100; ++Y) {
        byte *ATan = (byte *)&atanVal256[Y];
        for (int X = 0; X < 0x100; ++X) {
            float angle = atan2f(Y, X);
            *ATan       = (signed int)(angle * 40.743664f);
            ATan += 0x100;
        }
    }

    variableNames = variableNames_rev03;

    opcodeFunctionList = functionList_rev03;
}

void Compilerv4::CheckAliasText(char *text)
{
    if (FindStringToken(text, "publicalias", 1) == 0) {
        if (scriptValueListCount >= SCRIPT_VAR_COUNT_v4)
            PrintLog("WARNING: SCRIPT VALUE COUNT ABOVE MAXIMUM");

        ScriptVariableInfo *variable = &scriptValueList[scriptValueListCount];
        MEM_ZEROP(variable);

        int textStrPos = 11;
        int varStrPos  = 0;
        int parseMode  = 0;

        while (text[textStrPos]) {
            switch (parseMode) {
                default: break;

                case 0:
                    if (text[textStrPos] == ':') {
                        textStrPos++;
                        variable->value[varStrPos] = 0;
                        varStrPos                  = 0;
                        parseMode                  = 1;
                    }
                    else {
                        variable->value[varStrPos++] = text[textStrPos++];
                    }
                    break;

                case 1: variable->name[varStrPos++] = text[textStrPos++]; break;
            }
        }

        variable->access = ACCESS_PUBLIC;

        for (int v = 0; v < scriptValueListCount; ++v) {
            if (StrComp(scriptValueList[v].name, variable->name))
                PrintLog(
                    QString("WARNING: Variable Name '%1' has already been used!").arg(variable->name));
        }

        ++scriptValueListCount;
    }
    else if (FindStringToken(text, "privatealias", 1) == 0) {
        if (scriptValueListCount >= SCRIPT_VAR_COUNT_v4)
            PrintLog("WARNING: SCRIPT VALUE COUNT ABOVE MAXIMUM");

        ScriptVariableInfo *variable = &scriptValueList[scriptValueListCount];
        MEM_ZEROP(variable);

        int textStrPos = 12;
        int varStrPos  = 0;
        int parseMode  = 0;

        while (text[textStrPos]) {
            switch (parseMode) {
                default: break;

                case 0:
                    if (text[textStrPos] == ':') {
                        textStrPos++;
                        variable->value[varStrPos] = 0;
                        varStrPos                  = 0;
                        parseMode                  = 1;
                    }
                    else {
                        variable->value[varStrPos++] = text[textStrPos++];
                    }
                    break;

                case 1: variable->name[varStrPos++] = text[textStrPos++]; break;
            }
        }

        variable->access = ACCESS_PRIVATE;

        for (int v = 0; v < scriptValueListCount; ++v) {
            if (StrComp(scriptValueList[v].name, variable->name))
                PrintLog(
                    QString("WARNING: Variable Name '%1' has already been used!").arg(variable->name));
        }

        ++scriptValueListCount;
    }
}
void Compilerv4::CheckStaticText(char *text)
{
    if (FindStringToken(text, "publicvalue", 1) == 0) {
        if (scriptValueListCount >= SCRIPT_VAR_COUNT_v4)
            PrintLog("WARNING: SCRIPT VALUE COUNT ABOVE MAXIMUM");

        ScriptVariableInfo *variable = &scriptValueList[scriptValueListCount];
        MEM_ZEROP(variable);

        int textStrPos = 11;
        int varStrPos  = 0;
        int parseMode  = 0;

        StrCopy(variable->value, "0"); // default value is 0
        while (text[textStrPos]) {
            switch (parseMode) {
                default: break;

                case 0:
                    if (text[textStrPos] == '=') {
                        textStrPos++;
                        variable->name[varStrPos] = 0;
                        varStrPos                 = 0;
                        parseMode                 = 1;
                    }
                    else {
                        variable->name[varStrPos++] = text[textStrPos++];
                    }
                    break;

                case 1: variable->value[varStrPos++] = text[textStrPos++]; break;
            }
        }

        variable->access = ACCESS_PUBLIC;

        if (!ConvertStringToInteger(variable->value, &scriptCode[scriptCodePos]))
            scriptCode[scriptCodePos] = 0;

        StrCopy(variable->value, "local[");
        AppendIntegerToString(variable->value, scriptCodePos++);
        StrAdd(variable->value, "]");

        for (int v = 0; v < scriptValueListCount; ++v) {
            if (StrComp(scriptValueList[v].name, variable->name))
                PrintLog(
                    QString("WARNING: Variable Name '%1' has already been used!").arg(variable->name));
        }

        ++scriptValueListCount;
    }
    else if (FindStringToken(text, "privatevalue", 1) == 0) {
        if (scriptValueListCount >= SCRIPT_VAR_COUNT_v4)
            PrintLog("WARNING: SCRIPT VALUE COUNT ABOVE MAXIMUM");

        ScriptVariableInfo *variable = &scriptValueList[scriptValueListCount];
        MEM_ZEROP(variable);

        int textStrPos = 12;
        int varStrPos  = 0;
        int parseMode  = 0;

        StrCopy(variable->value, "0"); // default value is 0
        while (text[textStrPos]) {
            switch (parseMode) {
                default: break;

                case 0:
                    if (text[textStrPos] == '=') {
                        textStrPos++;
                        variable->name[varStrPos] = 0;
                        varStrPos                 = 0;
                        parseMode                 = 1;
                    }
                    else {
                        variable->name[varStrPos++] = text[textStrPos++];
                    }
                    break;

                case 1: variable->value[varStrPos++] = text[textStrPos++]; break;
            }
        }

        variable->access = ACCESS_PRIVATE;

        if (!ConvertStringToInteger(variable->value, &scriptCode[scriptCodePos]))
            scriptCode[scriptCodePos] = 0;

        StrCopy(variable->value, "local[");
        AppendIntegerToString(variable->value, scriptCodePos++);
        StrAdd(variable->value, "]");

        for (int v = 0; v < scriptValueListCount; ++v) {
            if (StrComp(scriptValueList[v].name, variable->name))
                PrintLog(
                    QString("WARNING: Variable Name '%1' has already been used!").arg(variable->name));
        }

        ++scriptValueListCount;
    }
}
bool Compilerv4::CheckTableText(char *text)
{
    bool hasValues = false;

    if (FindStringToken(text, "publictable", 1) == 0) {
        if (scriptValueListCount >= SCRIPT_VAR_COUNT_v4)
            PrintLog("WARNING: SCRIPT VALUE COUNT ABOVE MAXIMUM");

        ScriptVariableInfo *variable = &scriptValueList[scriptValueListCount];
        MEM_ZEROP(variable);

        int textStrPos = 11;
        int varStrPos  = 0;

        while (text[textStrPos]) {
            if (text[textStrPos] == '[' || text[textStrPos] == ']') {
                variable->name[varStrPos] = 0;
                textStrPos++;
                break;
            }
            else {
                variable->name[varStrPos++] = text[textStrPos++];
            }
        }

        if (FindStringToken(text, "]", 1) < 1) {
            // has default values, we'll stop here and read stuff in a seperate mode
            scriptCode[scriptCodePos] = 0;
            StrCopy(variable->value, "");
            AppendIntegerToString(variable->value, scriptCodePos);
            scriptCodeOffset = scriptCodePos++;
            hasValues        = true;
        }
        else {
            // no default values, just an array size

            varStrPos = 0;
            while (text[textStrPos]) {
                if (text[textStrPos] == '[' || text[textStrPos] == ']') {
                    variable->value[varStrPos] = 0;
                    textStrPos++;
                    break;
                }
                else {
                    variable->value[varStrPos++] = text[textStrPos++];
                }
            }

            // array size can be an variable (alias), how cool!
            for (int v = 0; v < scriptValueListCount; ++v) {
                if (StrComp(variable->value, scriptValueList[v].name))
                    StrCopy(variable->value, scriptValueList[v].value);
            }

            if (!ConvertStringToInteger(variable->value, &scriptCode[scriptCodePos])) {
                scriptCode[scriptCodePos] = 1;
                PrintLog("WARNING: Unable to parse table size!");
            }

            StrCopy(variable->value, "");
            AppendIntegerToString(variable->value, scriptCodePos);

            int valueCount = scriptCode[scriptCodePos++];
            for (int v = 0; v < valueCount; ++v) scriptCode[scriptCodePos++] = 0;
        }

        variable->access = ACCESS_PUBLIC;
        scriptValueListCount++;
    }
    else if (FindStringToken(text, "privatetable", 1) == 0) {
        if (scriptValueListCount >= SCRIPT_VAR_COUNT_v4)
            PrintLog("WARNING: SCRIPT VALUE COUNT ABOVE MAXIMUM");

        ScriptVariableInfo *variable = &scriptValueList[scriptValueListCount];
        MEM_ZEROP(variable);

        int textStrPos = 12;
        int varStrPos  = 0;

        while (text[textStrPos]) {
            if (text[textStrPos] == '[' || text[textStrPos] == ']') {
                variable->name[varStrPos] = 0;
                textStrPos++;
                break;
            }
            else {
                variable->name[varStrPos++] = text[textStrPos++];
            }
        }

        if (FindStringToken(text, "]", 1) < 1) {
            // has default values, we'll stop here and read stuff in a seperate mode
            scriptCode[scriptCodePos] = 0;
            StrCopy(variable->value, "");
            AppendIntegerToString(variable->value, scriptCodePos);
            scriptCodeOffset = scriptCodePos++;
            hasValues        = true;
        }
        else {
            // no default values, just an array size

            varStrPos = 0;
            while (text[textStrPos]) {
                if (text[textStrPos] == '[' || text[textStrPos] == ']') {
                    variable->value[varStrPos] = 0;
                    textStrPos++;
                    break;
                }
                else {
                    variable->value[varStrPos++] = text[textStrPos++];
                }
            }

            // array size can be an variable (alias), how cool!
            for (int v = 0; v < scriptValueListCount; ++v) {
                if (StrComp(variable->value, scriptValueList[v].name))
                    StrCopy(variable->value, scriptValueList[v].value);
            }

            if (!ConvertStringToInteger(variable->value, &scriptCode[scriptCodePos])) {
                scriptCode[scriptCodePos] = 1;
                PrintLog("WARNING: Unable to parse table size!");
            }

            StrCopy(variable->value, "");
            AppendIntegerToString(variable->value, scriptCodePos);

            int valueCount = scriptCode[scriptCodePos++];
            for (int v = 0; v < valueCount; ++v) scriptCode[scriptCodePos++] = 0;
        }

        variable->access = ACCESS_PRIVATE;
        scriptValueListCount++;
    }

    return hasValues;
}
void Compilerv4::ConvertArithmaticSyntax(char *text)
{
    int token  = 0;
    int offset = 0;
    int findID = 0;
    char dest[260];

    for (int i = FUNC_EQUAL; i <= FUNC_MOD; ++i) {
        findID = FindStringToken(text, scriptEvaluationTokens[i - 1], 1);
        if (findID > -1) {
            offset = findID;
            token  = i;
        }
    }

    if (token > 0) {
        StrCopy(dest, opcodeFunctionList[token].name);
        StrAdd(dest, "(");
        findID = StrLength(dest);
        for (int i = 0; i < offset; ++i) dest[findID++] = text[i];
        if (opcodeFunctionList[token].opcodeSize > 1) {
            dest[findID] = ',';
            int len      = StrLength(scriptEvaluationTokens[token - 1]);
            offset += len;
            ++findID;
            while (text[offset]) dest[findID++] = text[offset++];
        }
        dest[findID] = 0;
        StrAdd(dest, ")");
        StrCopy(text, dest);
    }
}
void Compilerv4::ConvertConditionalStatement(char *text)
{
    char dest[260];
    int compareOp  = -1;
    int strPos     = 0;
    int destStrPos = 0;

    if (FindStringToken(text, "if", 1) == 0) {
        for (int i = 0; i < 6; ++i) {
            destStrPos = FindStringToken(text, scriptEvaluationTokens[i + FUNC_MOD], 1);
            if (destStrPos > -1) {
                strPos    = destStrPos;
                compareOp = i;
            }
        }

        if (compareOp > -1) {
            text[strPos] = ',';
            StrCopy(dest, opcodeFunctionList[compareOp + FUNC_IFEQUAL].name);
            StrAdd(dest, "(");
            AppendIntegerToString(dest, jumpTablePos - jumpTableOffset);
            StrAdd(dest, ",");

            destStrPos = StrLength(dest);
            for (int i = 2; text[i]; ++i) {
                if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                    dest[destStrPos++] = text[i];
            }
            dest[destStrPos] = 0;

            StrAdd(dest, ")");
            StrCopy(text, dest);

            jumpTableStack[++jumpTableStackPos] = jumpTablePos;
            jumpTable[jumpTablePos++]           = -1;
            jumpTable[jumpTablePos++]           = 0;
        }
    }
    else if (FindStringToken(text, "while", 1) == 0) {
        for (int i = 0; i < 6; ++i) {
            destStrPos = FindStringToken(text, scriptEvaluationTokens[i + FUNC_MOD], 1);
            if (destStrPos > -1) {
                strPos    = destStrPos;
                compareOp = i;
            }
        }

        if (compareOp > -1) {
            text[strPos] = ',';
            StrCopy(dest, opcodeFunctionList[compareOp + FUNC_WEQUAL].name);
            StrAdd(dest, "(");
            AppendIntegerToString(dest, jumpTablePos - jumpTableOffset);
            StrAdd(dest, ",");

            destStrPos = StrLength(dest);
            for (int i = 5; text[i]; ++i) {
                if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                    dest[destStrPos++] = text[i];
            }
            dest[destStrPos] = 0;

            StrAdd(dest, ")");
            StrCopy(text, dest);

            jumpTableStack[++jumpTableStackPos] = jumpTablePos;
            jumpTable[jumpTablePos++]           = scriptCodePos - scriptCodeOffset;
            jumpTable[jumpTablePos++]           = 0;
        }
    }
    else if (FindStringToken(text, "foreach", 1) == 0) {
        int argStrPos = FindStringToken(text, ",", 2);

        if (argStrPos > -1) {
            StrCopy(dest, opcodeFunctionList[text[argStrPos + 2] == 'C' ? (int)FUNC_FOREACHACTIVE
                                                                        : (int)FUNC_FOREACHALL]
                              .name);
            StrAdd(dest, "(");
            AppendIntegerToString(dest, jumpTablePos - jumpTableOffset);
            StrAdd(dest, ",");

            destStrPos = StrLength(dest);
            for (int i = 7; text[i] && i < argStrPos; ++i) {
                if (text[i] != '(' && text[i] != ')')
                    dest[destStrPos++] = text[i];
            }
            dest[destStrPos] = 0;

            StrAdd(dest, ")");
            StrCopy(text, dest);

            jumpTableStack[++jumpTableStackPos] = jumpTablePos;
            jumpTable[jumpTablePos++]           = scriptCodePos - scriptCodeOffset;
            jumpTable[jumpTablePos++]           = 0;
        }
    }
}
bool Compilerv4::ConvertSwitchStatement(char *text)
{
    if (FindStringToken(text, "switch", 1) != 0)
        return false;

    char switchText[260];
    StrCopy(switchText, "switch");
    StrAdd(switchText, "(");
    AppendIntegerToString(switchText, jumpTablePos - jumpTableOffset);
    StrAdd(switchText, ",");
    int pos = StrLength(switchText);
    for (int i = 6; text[i]; ++i) {
        if (text[i] != '=' && text[i] != '(' && text[i] != ')')
            switchText[pos++] = text[i];
    }
    switchText[pos] = 0;
    StrAdd(switchText, ")");
    StrCopy(text, switchText);
    jumpTableStack[++jumpTableStackPos] = jumpTablePos;
    jumpTable[jumpTablePos++]           = 0x10000;
    jumpTable[jumpTablePos++]           = -0x10000;
    jumpTable[jumpTablePos++]           = -1;
    jumpTable[jumpTablePos++]           = 0;

    return true;
}
void Compilerv4::ConvertFunctionText(char *text)
{
    char arrayStr[0x80];
    char funcName[132];

    int opcode     = 0;
    int opcodeSize = 0;
    int textPos    = 0;
    int namePos    = 0;

    for (namePos = 0; text[namePos] != '(' && text[namePos]; ++namePos)
        funcName[namePos] = text[namePos];
    funcName[namePos] = 0;

    for (int i = 0; i < FUNC_MAX_CNT; ++i) {
        if (StrComp(funcName, opcodeFunctionList[i].name)) {
            opcode     = i;
            opcodeSize = opcodeFunctionList[i].opcodeSize;
            textPos    = StrLength(opcodeFunctionList[i].name);
            i          = FUNC_MAX_CNT;
        }
    }

    if (opcode <= 0) {
        scriptError = true;
        errorMsg    = "OPCODE NOT FOUND";
        errorPos    = funcName;
        errorLine   = lineID;
    }
    else {
        scriptCode[scriptCodePos++] = opcode;
        if (StrComp("else", opcodeFunctionList[opcode].name))
            jumpTable[jumpTableStack[jumpTableStackPos]] = scriptCodePos - scriptCodeOffset;

        if (StrComp("endif", opcodeFunctionList[opcode].name) == 1) {
            int jPos            = jumpTableStack[jumpTableStackPos];
            jumpTable[jPos + 1] = scriptCodePos - scriptCodeOffset;
            if (jumpTable[jPos] == -1)
                jumpTable[jPos] = (scriptCodePos - scriptCodeOffset) - 1;
            --jumpTableStackPos;
        }

        if (StrComp("endswitch", opcodeFunctionList[opcode].name)) {
            int jPos            = jumpTableStack[jumpTableStackPos];
            jumpTable[jPos + 3] = scriptCodePos - scriptCodeOffset;
            if (jumpTable[jPos + 2] == -1) {
                jumpTable[jPos + 2] = (scriptCodePos - scriptCodeOffset) - 1;
                int caseCnt         = abs(jumpTable[jPos + 1] - jumpTable[jPos]) + 1;

                int jOffset = jPos + 4;
                for (int c = 0; c < caseCnt; ++c) {
                    if (jumpTable[jOffset + c] < 0)
                        jumpTable[jOffset + c] = jumpTable[jPos + 2];
                }
            }
            --jumpTableStackPos;
        }

        if (StrComp("loop", opcodeFunctionList[opcode].name)
            || StrComp("next", opcodeFunctionList[opcode].name)) {
            jumpTable[jumpTableStack[jumpTableStackPos--] + 1] = scriptCodePos - scriptCodeOffset;
        }

        for (int i = 0; i < opcodeSize; ++i) {
            ++textPos;
            int funcNamePos = 0;
            int mode        = 0;
            int prevMode    = 0;
            int arrayStrPos = 0;
            while (((text[textPos] != ',' && text[textPos] != ')') || mode == 2) && text[textPos]) {
                switch (mode) {
                    case 0: // normal
                        if (text[textPos] == '[')
                            mode = 1;
                        else if (text[textPos] == '"') {
                            prevMode                = mode;
                            mode                    = 2;
                            funcName[funcNamePos++] = '"';
                        }
                        else
                            funcName[funcNamePos++] = text[textPos];
                        ++textPos;
                        break;

                    case 1: // array val
                        if (text[textPos] == ']')
                            mode = 0;
                        else if (text[textPos] == '"') {
                            prevMode = mode;
                            mode     = 2;
                        }
                        else
                            arrayStr[arrayStrPos++] = text[textPos];
                        ++textPos;
                        break;

                    case 2: // string
                        if (text[textPos] == '"') {
                            mode                    = prevMode;
                            funcName[funcNamePos++] = '"';
                        }
                        else
                            funcName[funcNamePos++] = text[textPos];
                        ++textPos;
                        break;
                }
            }
            funcName[funcNamePos] = 0;
            arrayStr[arrayStrPos] = 0;

            for (int v = 0; v < scriptValueListCount; ++v) {
                if (StrComp(funcName, scriptValueList[v].name)) {
                    CopyAliasStr(funcName, scriptValueList[v].value, 0);
                    if (FindStringToken(scriptValueList[v].value, "[", 1) > -1)
                        CopyAliasStr(arrayStr, scriptValueList[v].value, 1);
                }
            }

            if (arrayStr[0]) {
                char arrStrBuf[0x80];
                int arrPos = 0;
                int bufPos = 0;
                if (arrayStr[0] == '+' || arrayStr[0] == '-')
                    ++arrPos;
                while (arrayStr[arrPos]) arrStrBuf[bufPos++] = arrayStr[arrPos++];
                arrStrBuf[bufPos] = 0;

                for (int v = 0; v < scriptValueListCount; ++v) {
                    if (StrComp(arrStrBuf, scriptValueList[v].name)) {
                        char pref = arrayStr[0];
                        CopyAliasStr(arrayStr, scriptValueList[v].value, 0);

                        if (pref == '+' || pref == '-') {
                            int len = StrLength(arrayStr);
                            for (int i = len; i >= 0; --i) arrayStr[i + 1] = arrayStr[i];
                            arrayStr[0] = pref;
                        }
                    }
                }
            }

            // Eg: temp0 = game.variable
            for (int v = 0; v < globalVariables.count(); ++v) {
                if (StrComp(funcName, globalVariables[v].toStdString().c_str())) {
                    StrCopy(funcName, "global");
                    arrayStr[0] = 0;
                    AppendIntegerToString(arrayStr, v);
                }
            }

            // Eg: temp0 = Function1
            for (int f = 0; f < functionCount; ++f) {
                if (StrComp(funcName, functionList[f].name)) {
                    funcName[0] = 0;
                    AppendIntegerToString(funcName, f);
                }
            }

            // Eg: temp0 = TypeName[Player Object]
            if (StrComp(funcName, "TypeName")) {
                funcName[0] = '0';
                funcName[1] = 0;

                int o = 0;
                for (; o < OBJECT_COUNT_v4; ++o) {
                    if (StrComp(arrayStr, typeNames[o])) {
                        funcName[0] = 0;
                        AppendIntegerToString(funcName, o);
                        break;
                    }
                }

                if (o == OBJECT_COUNT_v4)
                    PrintLog(QString("WARNING: Unknown typename \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
            }

            // Eg: temp0 = SfxName[Jump]
            if (StrComp(funcName, "SfxName")) {
                funcName[0] = '0';
                funcName[1] = 0;

                int s = 0;
                for (; s < SFX_COUNT_v4; ++s) {
                    if (StrComp(arrayStr, sfxNames[s])) {
                        funcName[0] = 0;
                        AppendIntegerToString(funcName, s);
                        break;
                    }
                }

                if (s == SFX_COUNT_v4)
                    PrintLog(QString("WARNING: Unknown sfxName \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
            }

            // Eg: temp0 = VarName[player.lives]
            if (StrComp(funcName, "VarName")) {
                funcName[0] = '0';
                funcName[1] = 0;

                int v = 0;
                for (; v < globalVariables.count(); ++v) {
                    if (StrComp(arrayStr, globalVariables[v].toStdString().c_str())) {
                        funcName[0] = 0;
                        AppendIntegerToString(funcName, v);
                        break;
                    }
                }

                if (v == globalVariables.count())
                    PrintLog(QString("WARNING: Unknown varName \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
            }

            // Eg: temp0 = AchievementName[Ring King]
            if (StrComp(funcName, "AchievementName")) {
                funcName[0] = '0';
                funcName[1] = 0;
            }

            // Eg: temp0 = PlayerName[SONIC]
            if (StrComp(funcName, "PlayerName")) {
                funcName[0] = '0';
                funcName[1] = 0;
            }

            // Eg: temp0 = StageName[R - GREEN HILL ZONE 1]
            if (StrComp(funcName, "StageName")) {
                funcName[0] = '0';
                funcName[1] = 0;
            }

            // Storing Values
            int constant = 0;
            if (ConvertStringToInteger(funcName, &constant)) {
                scriptCode[scriptCodePos++] = SCRIPTVAR_INTCONST;
                scriptCode[scriptCodePos++] = constant;
            }
            else if (funcName[0] == '"') {
                scriptCode[scriptCodePos++] = SCRIPTVAR_STRCONST;
                scriptCode[scriptCodePos++] = StrLength(funcName) - 2;

                int scriptTextPos = 1;
                arrayStrPos       = 0;
                while (scriptTextPos > -1) {
                    switch (arrayStrPos) {
                        case 0:
                            scriptCode[scriptCodePos] = funcName[scriptTextPos] << 24;
                            ++arrayStrPos;
                            break;

                        case 1:
                            scriptCode[scriptCodePos] += funcName[scriptTextPos] << 16;
                            ++arrayStrPos;
                            break;

                        case 2:
                            scriptCode[scriptCodePos] += funcName[scriptTextPos] << 8;
                            ++arrayStrPos;
                            break;

                        case 3:
                            scriptCode[scriptCodePos++] += funcName[scriptTextPos];
                            arrayStrPos = 0;
                            break;

                        default: break;
                    }

                    if (funcName[scriptTextPos] == '"') {
                        if (arrayStrPos > 0)
                            ++scriptCodePos;
                        scriptTextPos = -1;
                    }
                    else {
                        scriptTextPos++;
                    }
                }
            }
            else {
                scriptCode[scriptCodePos++] = SCRIPTVAR_VAR;

                if (arrayStr[0]) {
                    scriptCode[scriptCodePos] = VARARR_ARRAY;

                    if (arrayStr[0] == '+')
                        scriptCode[scriptCodePos] = VARARR_ENTNOPLUS1;

                    if (arrayStr[0] == '-')
                        scriptCode[scriptCodePos] = VARARR_ENTNOMINUS1;

                    ++scriptCodePos;

                    if (arrayStr[0] == '-' || arrayStr[0] == '+') {
                        for (int i = 0; i < StrLength(arrayStr); ++i) arrayStr[i] = arrayStr[i + 1];
                    }

                    if (ConvertStringToInteger(arrayStr, &constant) == 1) {
                        scriptCode[scriptCodePos++] = 0;
                        scriptCode[scriptCodePos++] = constant;
                    }
                    else {
                        if (StrComp(arrayStr, "arrayPos0"))
                            constant = 0;
                        if (StrComp(arrayStr, "arrayPos1"))
                            constant = 1;
                        if (StrComp(arrayStr, "arrayPos2"))
                            constant = 2;
                        if (StrComp(arrayStr, "arrayPos3"))
                            constant = 3;
                        if (StrComp(arrayStr, "arrayPos4"))
                            constant = 4;
                        if (StrComp(arrayStr, "arrayPos5"))
                            constant = 5;
                        if (StrComp(arrayStr, "arrayPos6"))
                            constant = 6;
                        if (StrComp(arrayStr, "arrayPos7"))
                            constant = 7;
                        if (StrComp(arrayStr, "tempObjectPos"))
                            constant = 8;

                        scriptCode[scriptCodePos++] = 1;
                        scriptCode[scriptCodePos++] = constant;
                    }
                }
                else {
                    scriptCode[scriptCodePos++] = VARARR_NONE;
                }

                constant = -1;
                for (int i = 0; i < VAR_MAX_CNT; ++i) {
                    if (StrComp(funcName, variableNames[i].name))
                        constant = i;
                }

                if (constant == -1 && !scriptError) {
                    scriptError = true;
                    errorMsg    = "OPERAND NOT FOUND";
                    errorPos    = funcName;
                    errorLine   = lineID;
                    constant    = 0;
                }

                scriptCode[scriptCodePos++] = constant;
            }
        }
    }
}
void Compilerv4::CheckCaseNumber(char *text)
{
    if (FindStringToken(text, "case", 1) != 0)
        return;

    char caseString[128];
    int caseStrPos = 0;
    char caseChar  = text[4];
    if (text[4]) {
        int textPos = 5;
        do {
            if (caseChar != ':')
                caseString[caseStrPos++] = caseChar;
            caseChar = text[textPos++];
        } while (caseChar);
    }
    else {
        caseStrPos = 0;
    }
    caseString[caseStrPos] = 0;

    bool foundValue = false;

    if (FindStringToken(caseString, "[", 1) >= 0) {
        char caseValue[0x80];
        char arrayStr[0x80];

        int textPos     = 0;
        int funcNamePos = 0;
        int mode        = 0;
        int arrayStrPos = 0;
        while (caseString[textPos] != ':' && caseString[textPos]) {
            if (mode) {
                if (caseString[textPos] == ']')
                    mode = 0;
                else
                    arrayStr[arrayStrPos++] = caseString[textPos];
                ++textPos;
            }
            else {
                if (caseString[textPos] == '[')
                    mode = 1;
                else
                    caseValue[funcNamePos++] = caseString[textPos];
                ++textPos;
            }
        }
        caseValue[funcNamePos] = 0;
        arrayStr[arrayStrPos]  = 0;

        // Eg: temp0 = TypeName[Player Object]
        if (StrComp(caseValue, "TypeName")) {
            caseValue[0] = '0';
            caseValue[1] = 0;

            int o = 0;
            for (; o < OBJECT_COUNT_v4; ++o) {
                if (StrComp(arrayStr, typeNames[o])) {
                    caseValue[0] = 0;
                    AppendIntegerToString(caseValue, o);
                    break;
                }
            }

            if (o == OBJECT_COUNT_v4)
                PrintLog(
                    QString("WARNING: Unknown typename \"%1\", on line %2").arg(arrayStr).arg(lineID));
        }

        // Eg: temp0 = SfxName[Jump]
        if (StrComp(caseValue, "SfxName")) {
            caseValue[0] = '0';
            caseValue[1] = 0;

            int s = 0;
            for (; s < SFX_COUNT_v4; ++s) {
                if (StrComp(arrayStr, sfxNames[s])) {
                    caseValue[0] = 0;
                    AppendIntegerToString(caseValue, s);
                    break;
                }
            }

            if (s == SFX_COUNT_v4)
                PrintLog(
                    QString("WARNING: Unknown sfxName \"%1\", on line %2").arg(arrayStr).arg(lineID));
        }

        // Eg: temp0 = VarName[player.lives]
        if (StrComp(caseValue, "VarName")) {
            caseValue[0] = '0';
            caseValue[1] = 0;

            int v = 0;
            for (; v < globalVariables.count(); ++v) {
                if (StrComp(arrayStr, globalVariables[v].toStdString().c_str())) {
                    caseValue[0] = 0;
                    AppendIntegerToString(caseValue, v);
                    break;
                }
            }

            if (v == globalVariables.count()) {
                PrintLog(
                    QString("WARNING: Unknown varName \"%1\", on line %2").arg(arrayStr).arg(lineID));
            }
        }

        // Eg: temp0 = AchievementName[Ring King]
        if (StrComp(caseValue, "AchievementName")) {
            caseValue[0] = '0';
            caseValue[1] = 0;
        }

        // Eg: temp0 = PlayerName[SONIC]
        if (StrComp(caseValue, "PlayerName")) {
            caseValue[0] = '0';
            caseValue[1] = 0;
        }

        // Eg: temp0 = StageName[R - GREEN HILL ZONE 1]
        if (StrComp(caseValue, "StageName")) {
            caseValue[0] = '0';
            caseValue[1] = 0;
        }

        StrCopy(caseString, caseValue);
        foundValue = true;
    }

    for (int a = 0; a < scriptValueListCount && !foundValue; ++a) {
        if (StrComp(scriptValueList[a].name, caseString)) {
            StrCopy(caseString, scriptValueList[a].value);
            break;
        }
    }

    int caseID = 0;
    if (ConvertStringToInteger(caseString, &caseID)) {
        int stackValue = jumpTableStack[jumpTableStackPos];
        if (caseID < jumpTable[stackValue])
            jumpTable[stackValue] = caseID;
        stackValue++;
        if (caseID > jumpTable[stackValue])
            jumpTable[stackValue] = caseID;
    }
    else {
        PrintLog(QString("WARNING: unable to convert case string \"%1\" to int, on line %2")
                     .arg(caseString)
                     .arg(lineID));
    }
}
bool Compilerv4::ReadSwitchCase(char *text)
{
    char caseText[0x80];
    if (FindStringToken(text, "case", 1) == 0) {
        int textPos       = 4;
        int caseStringPos = 0;
        while (text[textPos]) {
            if (text[textPos] != ':')
                caseText[caseStringPos++] = text[textPos];
            ++textPos;
        }
        caseText[caseStringPos] = 0;

        bool foundValue = false;
        if (FindStringToken(caseText, "[", 1) >= 0) {
            char caseValue[0x80];
            char arrayStr[0x80];

            int textPos     = 0;
            int funcNamePos = 0;
            int mode        = 0;
            int arrayStrPos = 0;
            while (caseText[textPos] != ':' && caseText[textPos]) {
                if (mode) {
                    if (caseText[textPos] == ']')
                        mode = 0;
                    else
                        arrayStr[arrayStrPos++] = caseText[textPos];
                    ++textPos;
                }
                else {
                    if (caseText[textPos] == '[')
                        mode = 1;
                    else
                        caseValue[funcNamePos++] = caseText[textPos];
                    ++textPos;
                }
            }
            caseValue[funcNamePos] = 0;
            arrayStr[arrayStrPos]  = 0;

            // Eg: temp0 = TypeName[Player Object]
            if (StrComp(caseValue, "TypeName")) {
                caseValue[0] = '0';
                caseValue[1] = 0;

                int o = 0;
                for (; o < OBJECT_COUNT_v4; ++o) {
                    if (StrComp(arrayStr, typeNames[o])) {
                        caseValue[0] = 0;
                        AppendIntegerToString(caseValue, o);
                        break;
                    }
                }

                if (o == OBJECT_COUNT_v4)
                    PrintLog(QString("WARNING: Unknown typename \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
            }

            // Eg: temp0 = SfxName[Jump]
            if (StrComp(caseValue, "SfxName")) {
                caseValue[0] = '0';
                caseValue[1] = 0;

                int s = 0;
                for (; s < SFX_COUNT_v4; ++s) {
                    if (StrComp(arrayStr, sfxNames[s])) {
                        caseValue[0] = 0;
                        AppendIntegerToString(caseValue, s);
                        break;
                    }
                }

                if (s == SFX_COUNT_v4)
                    PrintLog(QString("WARNING: Unknown sfxName \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
            }

            // Eg: temp0 = VarName[player.lives]
            if (StrComp(caseValue, "VarName")) {
                caseValue[0] = '0';
                caseValue[1] = 0;

                int v = 0;
                for (; v < globalVariables.count(); ++v) {
                    if (StrComp(arrayStr, globalVariables[v].toStdString().c_str())) {
                        caseValue[0] = 0;
                        AppendIntegerToString(caseValue, v);
                        break;
                    }
                }

                if (v == globalVariables.count())
                    PrintLog(QString("WARNING: Unknown varName \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
            }

            // Eg: temp0 = AchievementName[Ring King]
            if (StrComp(caseValue, "AchievementName")) {
                caseValue[0] = '0';
                caseValue[1] = 0;
            }

            // Eg: temp0 = PlayerName[SONIC]
            if (StrComp(caseValue, "PlayerName")) {
                caseValue[0] = '0';
                caseValue[1] = 0;
            }

            // Eg: temp0 = StageName[R - GREEN HILL ZONE 1]
            if (StrComp(caseValue, "StageName")) {
                caseValue[0] = '0';
                caseValue[1] = 0;
            }
            StrCopy(caseText, caseValue);
            foundValue = true;
        }

        for (int v = 0; v < scriptValueListCount && !foundValue; ++v) {
            if (StrComp(caseText, scriptValueList[v].name)) {
                StrCopy(caseText, scriptValueList[v].value);
                break;
            }
        }

        int val = 0;

        int jPos    = jumpTableStack[jumpTableStackPos];
        int jOffset = jPos + 4;
        if (ConvertStringToInteger(caseText, &val))
            jumpTable[val - jumpTable[jPos] + jOffset] = scriptCodePos - scriptCodeOffset;
        else
            PrintLog(QString("WARNING: unable to read case string \"%1\" as an int, on line %2")
                         .arg(caseText)
                         .arg(lineID));

        return true;
    }
    else if (FindStringToken(text, "default", 1) == 0) {
        int jumpTablepos            = jumpTableStack[jumpTableStackPos];
        jumpTable[jumpTablepos + 2] = scriptCodePos - scriptCodeOffset;
        int cnt                     = abs(jumpTable[jumpTablepos + 1] - jumpTable[jumpTablepos]) + 1;

        int jOffset = jumpTablepos + 4;
        for (int i = 0; i < cnt; ++i) {
            if (jumpTable[jOffset + i] < 0)
                jumpTable[jOffset + i] = scriptCodePos - scriptCodeOffset;
        }

        return true;
    }

    return false;
}
void Compilerv4::ReadTableValues(char *text)
{
    int textStrPos = 0;

    char valueBuffer[256];
    int valueBufferPos = 0;

    while (text[textStrPos]) {
        valueBuffer[valueBufferPos++] = text[textStrPos++];

        while (text[textStrPos] == ',') {
            valueBuffer[valueBufferPos] = 0;
            ++scriptCode[scriptCodeOffset];
            if (!ConvertStringToInteger(valueBuffer, &scriptCode[scriptCodePos])) {
                scriptCode[scriptCodePos] = 0;
                PrintLog(QString("WARNING: unable to parse table value \"%1\" as an int, on line %2")
                             .arg(valueBuffer)
                             .arg(lineID));
            }
            scriptCodePos++;
            valueBufferPos = 0;
            textStrPos++;
        }
    }

    if (StrLength(valueBuffer)) {
        valueBuffer[valueBufferPos] = 0;
        ++scriptCode[scriptCodeOffset];
        if (!ConvertStringToInteger(valueBuffer, &scriptCode[scriptCodePos])) {
            scriptCode[scriptCodePos] = 0;
            PrintLog(QString("WARNING: unable to parse table value \"%1\" as an int, on line %2")
                         .arg(valueBuffer)
                         .arg(lineID));
        }
        scriptCodePos++;
    }
}
void Compilerv4::CopyAliasStr(char *dest, char *text, bool arrayIndex)
{
    int textPos     = 0;
    int destPos     = 0;
    bool arrayValue = false;
    if (arrayIndex) {
        while (text[textPos]) {
            if (arrayValue) {
                if (text[textPos] == ']')
                    arrayValue = false;
                else
                    dest[destPos++] = text[textPos];
                ++textPos;
            }
            else {
                if (text[textPos] == '[')
                    arrayValue = true;
                ++textPos;
            }
        }
    }
    else {
        while (text[textPos]) {
            if (arrayValue) {
                if (text[textPos] == ']')
                    arrayValue = false;
                ++textPos;
            }
            else {
                if (text[textPos] == '[')
                    arrayValue = true;
                else
                    dest[destPos++] = text[textPos];
                ++textPos;
            }
        }
    }
    dest[destPos] = 0;
}

void Compilerv4::ParseScriptFile(QString scriptName, int scriptID, bool inEditor)
{
    this->inEditor = inEditor;

    jumpTableStackPos = 0;
    lineID            = 0;

    for (int f = 0; f < functionCount; ++f) {
        if (functionList[f].access != ACCESS_PUBLIC)
            StrCopy(functionList[f].name, "");
    }

    int newScriptValueCount = COMMON_SCRIPT_VAR_COUNT_v4;
    for (int v = COMMON_SCRIPT_VAR_COUNT_v4; v < scriptValueListCount; ++v) {
        if (scriptValueList[v].access != ACCESS_PUBLIC) {
            StrCopy(scriptValueList[v].name, "");
        }
        else {
            if (newScriptValueCount != v)
                memcpy(&scriptValueList[newScriptValueCount], &scriptValueList[v],
                       sizeof(ScriptVariableInfo));

            newScriptValueCount++;
        }
    }
    scriptValueListCount = newScriptValueCount;

    for (int v = scriptValueListCount; v < SCRIPT_VAR_COUNT_v4; ++v) {
        MEM_ZERO(scriptValueList[v]);
    }

    scriptError = false;

    Reader reader(scriptName);
    if (reader.initialised) {
        int storePos   = 0;
        int readMode   = READMODE_NORMAL;
        int parseMode  = PARSEMODE_SCOPELESS;
        char prevChar  = 0;
        char curChar   = 0;
        int switchDeep = 0;

        while (readMode < READMODE_EOF) {
            int textPos               = 0;
            readMode                  = READMODE_NORMAL;
            bool disableLineIncrement = false;

            while (readMode < READMODE_ENDLINE) {
                prevChar = curChar;
                curChar  = reader.read<char>();

                if (readMode == READMODE_STRING) {
                    if (curChar == '\t' || curChar == '\r' || curChar == '\n' || curChar == ';'
                        || readMode >= READMODE_COMMENTLINE) {
                        if ((curChar == '\n' && prevChar != '\r')
                            || (curChar == '\n' && prevChar == '\r')) {
                            readMode            = READMODE_ENDLINE;
                            scriptText[textPos] = 0;
                            if (curChar == ';')
                                disableLineIncrement = true;
                        }
                    }
                    else if (curChar != '/' || textPos <= 0) {
                        scriptText[textPos++] = curChar;
                        if (curChar == '"')
                            readMode = READMODE_NORMAL;
                    }
                    else if (curChar == '/' && prevChar == '/') {
                        readMode              = READMODE_COMMENTLINE;
                        scriptText[--textPos] = 0;
                    }
                    else {
                        scriptText[textPos++] = curChar;
                    }
                }
                else if (curChar == ' ' || curChar == '\t' || curChar == '\r' || curChar == '\n'
                         || curChar == ';' || readMode >= READMODE_COMMENTLINE) {
                    if ((curChar == '\n' && prevChar != '\r') || (curChar == '\n' && prevChar == '\r')
                        || curChar == ';') {
                        // don't read commas as endline in comments
                        if (readMode < READMODE_COMMENTLINE || curChar != ';') {
                            readMode            = READMODE_ENDLINE;
                            scriptText[textPos] = 0;
                            if (curChar == ';')
                                disableLineIncrement = true;
                        }
                    }
                }
                else if (curChar != '/' || textPos <= 0) {
                    scriptText[textPos++] = curChar;
                    if (curChar == '"' && !readMode)
                        readMode = READMODE_STRING;
                }
                else if (curChar == '/' && prevChar == '/') {
                    readMode              = READMODE_COMMENTLINE;
                    scriptText[--textPos] = 0;
                }
                else {
                    scriptText[textPos++] = curChar;
                }

                if (reader.isEOF()) {
                    scriptText[textPos] = 0;
                    readMode            = READMODE_EOF;
                }
            }

            switch (parseMode) {
                case PARSEMODE_SCOPELESS:
                    if (!disableLineIncrement)
                        ++lineID;

                    CheckAliasText(scriptText);
                    CheckStaticText(scriptText);

                    if (CheckTableText(scriptText)) {
                        parseMode = PARSEMODE_TABLEREAD;
                        StrCopy(scriptText, "");
                    }

                    if (inEditor) {
                        if (StrComp(scriptText, "eventRSDKDraw")) {
                            parseMode                                              = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventRSDKDraw.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].eventRSDKDraw.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                       = scriptCodePos;
                            jumpTableOffset                                        = jumpTablePos;
                        }
                        if (StrComp(scriptText, "eventRSDKLoad")) {
                            parseMode                                              = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventRSDKLoad.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].eventRSDKLoad.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                       = scriptCodePos;
                            jumpTableOffset                                        = jumpTablePos;
                        }
                        if (StrComp(scriptText, "eventRSDKEdit")) {
                            parseMode                                              = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventRSDKEdit.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].eventRSDKEdit.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                       = scriptCodePos;
                            jumpTableOffset                                        = jumpTablePos;
                        }
                    }
                    else {
                        if (StrComp(scriptText, "eventObjectUpdate")) {
                            parseMode                                            = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventUpdate.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].eventUpdate.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                     = scriptCodePos;
                            jumpTableOffset                                      = jumpTablePos;
                        }

                        if (StrComp(scriptText, "eventObjectDraw")) {
                            parseMode                                          = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventDraw.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].eventDraw.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                   = scriptCodePos;
                            jumpTableOffset                                    = jumpTablePos;
                        }

                        if (StrComp(scriptText, "eventObjectStartup")) {
                            parseMode                                             = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventStartup.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].eventStartup.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                      = scriptCodePos;
                            jumpTableOffset                                       = jumpTablePos;
                        }
                    }

                    if (FindStringToken(scriptText, "reservefunction", 1) == 0) { // forward decl
                        char funcName[0x40];
                        for (textPos = 15; scriptText[textPos]; ++textPos)
                            funcName[textPos - 15] = scriptText[textPos];
                        funcName[textPos - 15] = 0;
                        int funcID             = -1;
                        for (int f = 0; f < functionCount; ++f) {
                            if (StrComp(funcName, functionList[f].name))
                                funcID = f;
                        }

                        if (functionCount < FUNCTION_COUNT_v4 && funcID == -1) {
                            StrCopy(functionList[functionCount++].name, funcName);
                        }
                        else {
                            PrintLog(QString("WARNING: Function %1 has already been reserved!")
                                         .arg(funcName));
                        }

                        parseMode = PARSEMODE_SCOPELESS;
                    }
                    else if (FindStringToken(scriptText, "publicfunction", 1)
                             == 0) { // regular public decl
                        char funcName[0x40];
                        for (textPos = 14; scriptText[textPos]; ++textPos)
                            funcName[textPos - 14] = scriptText[textPos];

                        funcName[textPos - 14] = 0;
                        int funcID             = -1;
                        for (int f = 0; f < functionCount; ++f) {
                            if (StrComp(funcName, functionList[f].name))
                                funcID = f;
                        }

                        if (funcID <= -1) {
                            if (functionCount >= FUNCTION_COUNT_v4) {
                                parseMode = PARSEMODE_SCOPELESS;
                            }
                            else {
                                StrCopy(functionList[functionCount].name, funcName);
                                functionList[functionCount].access            = ACCESS_PUBLIC;
                                functionList[functionCount].ptr.scriptCodePtr = scriptCodePos;
                                functionList[functionCount].ptr.jumpTablePtr  = jumpTablePos;

                                scriptCodeOffset = scriptCodePos;
                                jumpTableOffset  = jumpTablePos;
                                parseMode        = PARSEMODE_FUNCTION;
                                ++functionCount;
                            }
                        }
                        else {
                            StrCopy(functionList[funcID].name, funcName);
                            functionList[funcID].access            = ACCESS_PUBLIC;
                            functionList[funcID].ptr.scriptCodePtr = scriptCodePos;
                            functionList[funcID].ptr.jumpTablePtr  = jumpTablePos;

                            scriptCodeOffset = scriptCodePos;
                            jumpTableOffset  = jumpTablePos;
                            parseMode        = PARSEMODE_FUNCTION;
                        }
                    }
                    else if (FindStringToken(scriptText, "privatefunction", 1)
                             == 0) { // regular private decl
                        char funcName[0x40];
                        for (textPos = 15; scriptText[textPos]; ++textPos)
                            funcName[textPos - 15] = scriptText[textPos];

                        funcName[textPos - 15] = 0;
                        int funcID             = -1;
                        for (int f = 0; f < functionCount; ++f) {
                            if (StrComp(funcName, functionList[f].name))
                                funcID = f;
                        }

                        if (funcID <= -1) {
                            if (functionCount >= FUNCTION_COUNT_v4) {
                                parseMode = PARSEMODE_SCOPELESS;
                            }
                            else {
                                StrCopy(functionList[functionCount].name, funcName);
                                functionList[functionCount].access            = ACCESS_PRIVATE;
                                functionList[functionCount].ptr.scriptCodePtr = scriptCodePos;
                                functionList[functionCount].ptr.jumpTablePtr  = jumpTablePos;

                                scriptCodeOffset = scriptCodePos;
                                jumpTableOffset  = jumpTablePos;
                                parseMode        = PARSEMODE_FUNCTION;
                                ++functionCount;
                            }
                        }
                        else {
                            StrCopy(functionList[funcID].name, funcName);
                            functionList[funcID].access            = ACCESS_PRIVATE;
                            functionList[funcID].ptr.scriptCodePtr = scriptCodePos;
                            functionList[funcID].ptr.jumpTablePtr  = jumpTablePos;

                            scriptCodeOffset = scriptCodePos;
                            jumpTableOffset  = jumpTablePos;
                            parseMode        = PARSEMODE_FUNCTION;
                        }
                    }
                    break;

                case PARSEMODE_PLATFORMSKIP:
                    if (!disableLineIncrement)
                        ++lineID;

                    if (FindStringToken(scriptText, "#endplatform", 1) == 0)
                        parseMode = PARSEMODE_FUNCTION;
                    break;

                case PARSEMODE_FUNCTION:
                    if (!disableLineIncrement)
                        ++lineID;

                    if (scriptText[0]) {
                        if (StrComp(scriptText, "endevent")) {
                            scriptCode[scriptCodePos++] = FUNC_END;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
                        else if (StrComp(scriptText, "endfunction")) {
                            scriptCode[scriptCodePos++] = FUNC_RETURN;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
                        else if (FindStringToken(scriptText, "#platform:", 1) == 0) {
                            if (FindStringToken(scriptText, gamePlatform, 1) == -1
                                && FindStringToken(scriptText, gameRenderType, 1) == -1
                                && FindStringToken(scriptText, gameHapticSetting, 1) == -1
                                && FindStringToken(scriptText, releaseType, 1)
                                       == -1 // general flag for standalone/origins contnet switching
                                && FindStringToken(scriptText, "USE_DECOMP", 1)
                                       == -1 // general flag for decomp-only stuff
                            ) {
                                parseMode = PARSEMODE_PLATFORMSKIP;
                            }
                        }
                        else if (FindStringToken(scriptText, "#endplatform", 1) == -1) {
                            ConvertConditionalStatement(scriptText);
                            if (ConvertSwitchStatement(scriptText)) {
                                parseMode  = PARSEMODE_SWITCHREAD;
                                storePos   = (int)reader.tell();
                                switchDeep = 0;
                            }
                            ConvertArithmaticSyntax(scriptText);
                            if (!ReadSwitchCase(scriptText)) {
                                ConvertFunctionText(scriptText);
                                if (scriptError) {
                                    errorScr  = scriptName.toStdString().c_str();
                                    parseMode = PARSEMODE_ERROR;
                                }
                            }
                        }
                    }
                    break;

                case PARSEMODE_SWITCHREAD:
                    if (FindStringToken(scriptText, "switch", 1) == 0)
                        ++switchDeep;

                    if (switchDeep) {
                        if (FindStringToken(scriptText, "endswitch", 1) == 0)
                            --switchDeep;
                    }
                    else if (FindStringToken(scriptText, "endswitch", 1) == 0) {
                        reader.seek(storePos);
                        parseMode  = PARSEMODE_FUNCTION;
                        int jPos   = jumpTableStack[jumpTableStackPos];
                        switchDeep = abs(jumpTable[jPos + 1] - jumpTable[jPos]) + 1;
                        for (textPos = 0; textPos < switchDeep; ++textPos)
                            jumpTable[jumpTablePos++] = -1;
                    }
                    else {
                        CheckCaseNumber(scriptText);
                    }
                    break;

                case PARSEMODE_TABLEREAD:
                    if (!disableLineIncrement)
                        ++lineID;

                    if (FindStringToken(scriptText, "endtable", 1) == 0) {
                        parseMode = PARSEMODE_SCOPELESS;
                    }
                    else {
                        if (StrLength(scriptText) >= 1)
                            ReadTableValues(scriptText);

                        parseMode = PARSEMODE_TABLEREAD;
                    }
                    break;

                default: break;
            }
        }

        reader.close();
    }
}

void Compilerv4::ClearScriptData()
{
    memset(scriptCode, 0, SCRIPTCODE_COUNT_v4 * sizeof(int));
    memset(jumpTable, 0, JUMPTABLE_COUNT_v4 * sizeof(int));

    memset(foreachStack, -1, FORSTACK_COUNT_v4 * sizeof(int));
    memset(jumpTableStack, 0, JUMPSTACK_COUNT_v4 * sizeof(int));
    memset(functionStack, 0, FUNCSTACK_COUNT_v4 * sizeof(int));

    globalVariables.clear();

    scriptFrameCount = 0;

    // scriptCodePos     = 0;
    // jumpTablePos      = 0;
    jumpTableStackPos = 0;
    functionStackPos  = 0;

    scriptCodePos    = 0;
    scriptCodeOffset = 0;
    jumpTablePos     = 0;
    jumpTableOffset  = 0;

    functionCount = 0;

    lineID = 0;

    scriptValueListCount = COMMON_SCRIPT_VAR_COUNT_v4;
    for (int v = COMMON_SCRIPT_VAR_COUNT_v4; v < SCRIPT_VAR_COUNT_v4; ++v) {
        MEM_ZERO(scriptValueList[v]);
    }

    for (int o = 0; o < OBJECT_COUNT_v4; ++o) {
        ObjectScript *scriptInfo = &objectScriptList[o];

        scriptInfo->eventUpdate.scriptCodePtr  = SCRIPTCODE_COUNT_v4 - 1;
        scriptInfo->eventUpdate.jumpTablePtr   = JUMPTABLE_COUNT_v4 - 1;
        scriptInfo->eventDraw.scriptCodePtr    = SCRIPTCODE_COUNT_v4 - 1;
        scriptInfo->eventDraw.jumpTablePtr     = JUMPTABLE_COUNT_v4 - 1;
        scriptInfo->eventStartup.scriptCodePtr = SCRIPTCODE_COUNT_v4 - 1;
        scriptInfo->eventStartup.jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;

        scriptInfo->eventRSDKDraw.scriptCodePtr = SCRIPTCODE_COUNT_v4 - 1;
        scriptInfo->eventRSDKDraw.jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;
        scriptInfo->eventRSDKLoad.scriptCodePtr = SCRIPTCODE_COUNT_v4 - 1;
        scriptInfo->eventRSDKLoad.jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;
        scriptInfo->eventRSDKEdit.scriptCodePtr = SCRIPTCODE_COUNT_v4 - 1;
        scriptInfo->eventRSDKEdit.jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;

        typeNames[o][0] = 0;
    }

    for (int f = 0; f < FUNCTION_COUNT_v4; ++f) {
        functionList[f].ptr.scriptCodePtr = SCRIPTCODE_COUNT_v4 - 1;
        functionList[f].ptr.jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;
    }

    SetScriptTypeName("Blank Object", typeNames[0]);
}

void Compilerv4::WriteBytecode(QString path)
{
    bytecode = RSDKv4::Bytecode();

    bytecode.scriptCode.clear();
    for (int i = globalScriptCodeCount; i < scriptCodePos; ++i) {
        bytecode.scriptCode.append(scriptCode[i]);
    }

    bytecode.jumpTable.clear();
    for (int i = globalJumpTableCount; i < jumpTablePos; ++i) {
        bytecode.jumpTable.append(jumpTable[i]);
    }

    bytecode.scriptList.clear();
    for (int i = globalScriptCount; i < scriptCount; ++i) {
        RSDKv4::Bytecode::ScriptInfo scr;

        scr.update.scriptCodePos = objectScriptList[i].eventUpdate.scriptCodePtr;
        scr.update.jumpTablePos  = objectScriptList[i].eventUpdate.jumpTablePtr;

        scr.draw.scriptCodePos = objectScriptList[i].eventDraw.scriptCodePtr;
        scr.draw.jumpTablePos  = objectScriptList[i].eventDraw.jumpTablePtr;

        scr.startup.scriptCodePos = objectScriptList[i].eventStartup.scriptCodePtr;
        scr.startup.jumpTablePos  = objectScriptList[i].eventStartup.jumpTablePtr;

        bytecode.scriptList.append(scr);
    }

    bytecode.functionList.clear();
    for (int f = 0; f < functionCount; ++f) {
        RSDKv4::Bytecode::FunctionInfo func;
        func.scriptCodePos = functionList[f].ptr.scriptCodePtr;
        func.jumpTablePos  = functionList[f].ptr.jumpTablePtr;
        bytecode.functionList.append(func);
    }

    bytecode.write(path);
}

void Compilerv4::ProcessScript(int scriptCodeStart, int jumpTableStart, byte scriptEvent)
{
    bool running        = true;
    int scriptCodePtr   = scriptCodeStart;
    jumpTableStackPos   = 0;
    functionStackPos    = 0;
    foreachStackPos     = 0;
    SceneEditor *editor = (SceneEditor *)this->editor;

    while (running) {
        int opcode           = scriptCode[scriptCodePtr++];
        int opcodeSize       = opcodeFunctionList[opcode].opcodeSize;
        int scriptCodeOffset = scriptCodePtr;

        scriptText[0] = '\0';

        // Get Values
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = scriptCode[scriptCodePtr++];

            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (scriptCode[scriptCodePtr++]) {
                    case VARARR_NONE: arrayVal = objectEntityPos; break;

                    case VARARR_ARRAY:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal = scriptEng.arrayPosition[scriptCode[scriptCodePtr++]];
                        else
                            arrayVal = scriptCode[scriptCodePtr++];
                        break;

                    case VARARR_ENTNOPLUS1:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal =
                                scriptEng.arrayPosition[scriptCode[scriptCodePtr++]] + objectEntityPos;
                        else
                            arrayVal = scriptCode[scriptCodePtr++] + objectEntityPos;
                        break;

                    case VARARR_ENTNOMINUS1:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal =
                                objectEntityPos - scriptEng.arrayPosition[scriptCode[scriptCodePtr++]];
                        else
                            arrayVal = objectEntityPos - scriptCode[scriptCodePtr++];
                        break;

                    default: break;
                }

                // Allows the game to link to the editor properly
                Entity *entityPtr = &objectEntityList[arrayVal];
                if (editor) {
                    for (int e = 0; e < editor->viewer->entities.count(); ++e) {
                        if (arrayVal == editor->viewer->entities[e].slotID) {
                            entityPtr = &objectEntityList[editor->viewer->entities[e].gameEntitySlot];
                        }
                    }
                }

                // Variables
                switch (scriptCode[scriptCodePtr++]) {
                    default: break;
                    case VAR_TEMP0: scriptEng.operands[i] = scriptEng.tempValue[0]; break;
                    case VAR_TEMP1: scriptEng.operands[i] = scriptEng.tempValue[1]; break;
                    case VAR_TEMP2: scriptEng.operands[i] = scriptEng.tempValue[2]; break;
                    case VAR_TEMP3: scriptEng.operands[i] = scriptEng.tempValue[3]; break;
                    case VAR_TEMP4: scriptEng.operands[i] = scriptEng.tempValue[4]; break;
                    case VAR_TEMP5: scriptEng.operands[i] = scriptEng.tempValue[5]; break;
                    case VAR_TEMP6: scriptEng.operands[i] = scriptEng.tempValue[6]; break;
                    case VAR_TEMP7: scriptEng.operands[i] = scriptEng.tempValue[7]; break;
                    case VAR_CHECKRESULT: scriptEng.operands[i] = scriptEng.checkResult; break;
                    case VAR_ARRAYPOS0: scriptEng.operands[i] = scriptEng.arrayPosition[0]; break;
                    case VAR_ARRAYPOS1: scriptEng.operands[i] = scriptEng.arrayPosition[1]; break;
                    case VAR_ARRAYPOS2: scriptEng.operands[i] = scriptEng.arrayPosition[2]; break;
                    case VAR_ARRAYPOS3: scriptEng.operands[i] = scriptEng.arrayPosition[3]; break;
                    case VAR_ARRAYPOS4: scriptEng.operands[i] = scriptEng.arrayPosition[4]; break;
                    case VAR_ARRAYPOS5: scriptEng.operands[i] = scriptEng.arrayPosition[5]; break;
                    case VAR_ARRAYPOS6: scriptEng.operands[i] = scriptEng.arrayPosition[6]; break;
                    case VAR_ARRAYPOS7: scriptEng.operands[i] = scriptEng.arrayPosition[7]; break;
                    case VAR_GLOBAL: /*scriptEng.operands[i] = globalVariables[arrayVal];*/ break;
                    case VAR_LOCAL: scriptEng.operands[i] = scriptCode[arrayVal]; break;
                    case VAR_OBJECTENTITYPOS: scriptEng.operands[i] = arrayVal; break;
                    case VAR_OBJECTGROUPID: {
                        scriptEng.operands[i] = entityPtr->typeGroup;
                        break;
                    }
                    case VAR_OBJECTTYPE: {
                        scriptEng.operands[i] = entityPtr->type;
                        break;
                    }
                    case VAR_OBJECTPROPERTYVALUE: {
                        scriptEng.operands[i] = entityPtr->propertyValue;
                        break;
                    }
                    case VAR_OBJECTXPOS: {
                        scriptEng.operands[i] = entityPtr->XPos;
                        break;
                    }
                    case VAR_OBJECTYPOS: {
                        scriptEng.operands[i] = entityPtr->YPos;
                        break;
                    }
                    case VAR_OBJECTIXPOS: {
                        scriptEng.operands[i] = entityPtr->XPos >> 16;
                        break;
                    }
                    case VAR_OBJECTIYPOS: {
                        scriptEng.operands[i] = entityPtr->YPos >> 16;
                        break;
                    }
                    case VAR_OBJECTXVEL: {
                        scriptEng.operands[i] = entityPtr->XVelocity;
                        break;
                    }
                    case VAR_OBJECTYVEL: {
                        scriptEng.operands[i] = entityPtr->YVelocity;
                        break;
                    }
                    case VAR_OBJECTSPEED: {
                        scriptEng.operands[i] = entityPtr->speed;
                        break;
                    }
                    case VAR_OBJECTSTATE: {
                        scriptEng.operands[i] = entityPtr->state;
                        break;
                    }
                    case VAR_OBJECTROTATION: {
                        scriptEng.operands[i] = entityPtr->rotation;
                        break;
                    }
                    case VAR_OBJECTSCALE: {
                        scriptEng.operands[i] = entityPtr->scale;
                        break;
                    }
                    case VAR_OBJECTPRIORITY: {
                        scriptEng.operands[i] = entityPtr->priority;
                        break;
                    }
                    case VAR_OBJECTDRAWORDER: {
                        scriptEng.operands[i] = entityPtr->drawOrder;
                        break;
                    }
                    case VAR_OBJECTDIRECTION: {
                        scriptEng.operands[i] = entityPtr->direction;
                        break;
                    }
                    case VAR_OBJECTINKEFFECT: {
                        scriptEng.operands[i] = entityPtr->inkEffect;
                        break;
                    }
                    case VAR_OBJECTALPHA: {
                        scriptEng.operands[i] = entityPtr->alpha;
                        break;
                    }
                    case VAR_OBJECTFRAME: {
                        scriptEng.operands[i] = entityPtr->frame;
                        break;
                    }
                    case VAR_OBJECTANIMATION: {
                        scriptEng.operands[i] = entityPtr->animation;
                        break;
                    }
                    case VAR_OBJECTPREVANIMATION: {
                        scriptEng.operands[i] = entityPtr->prevAnimation;
                        break;
                    }
                    case VAR_OBJECTANIMATIONSPEED: {
                        scriptEng.operands[i] = entityPtr->animationSpeed;
                        break;
                    }
                    case VAR_OBJECTANIMATIONTIMER: {
                        scriptEng.operands[i] = entityPtr->animationTimer;
                        break;
                    }
                    case VAR_OBJECTANGLE: {
                        scriptEng.operands[i] = entityPtr->angle;
                        break;
                    }
                    case VAR_OBJECTLOOKPOSX: {
                        scriptEng.operands[i] = entityPtr->lookPosX;
                        break;
                    }
                    case VAR_OBJECTLOOKPOSY: {
                        scriptEng.operands[i] = entityPtr->lookPosY;
                        break;
                    }
                    case VAR_OBJECTCOLLISIONMODE: {
                        scriptEng.operands[i] = entityPtr->collisionMode;
                        break;
                    }
                    case VAR_OBJECTCOLLISIONPLANE: {
                        scriptEng.operands[i] = entityPtr->collisionPlane;
                        break;
                    }
                    case VAR_OBJECTCONTROLMODE: {
                        scriptEng.operands[i] = entityPtr->controlMode;
                        break;
                    }
                    case VAR_OBJECTCONTROLLOCK: {
                        scriptEng.operands[i] = entityPtr->controlLock;
                        break;
                    }
                    case VAR_OBJECTPUSHING: {
                        scriptEng.operands[i] = entityPtr->pushing;
                        break;
                    }
                    case VAR_OBJECTVISIBLE: {
                        scriptEng.operands[i] = entityPtr->visible;
                        break;
                    }
                    case VAR_OBJECTTILECOLLISIONS: {
                        scriptEng.operands[i] = entityPtr->tileCollisions;
                        break;
                    }
                    case VAR_OBJECTINTERACTION: {
                        scriptEng.operands[i] = entityPtr->objectInteractions;
                        break;
                    }
                    case VAR_OBJECTGRAVITY: {
                        scriptEng.operands[i] = entityPtr->gravity;
                        break;
                    }
                    case VAR_OBJECTUP: {
                        scriptEng.operands[i] = entityPtr->up;
                        break;
                    }
                    case VAR_OBJECTDOWN: {
                        scriptEng.operands[i] = entityPtr->down;
                        break;
                    }
                    case VAR_OBJECTLEFT: {
                        scriptEng.operands[i] = entityPtr->left;
                        break;
                    }
                    case VAR_OBJECTRIGHT: {
                        scriptEng.operands[i] = entityPtr->right;
                        break;
                    }
                    case VAR_OBJECTJUMPPRESS: {
                        scriptEng.operands[i] = entityPtr->jumpPress;
                        break;
                    }
                    case VAR_OBJECTJUMPHOLD: {
                        scriptEng.operands[i] = entityPtr->jumpHold;
                        break;
                    }
                    case VAR_OBJECTSCROLLTRACKING: {
                        scriptEng.operands[i] = entityPtr->trackScroll;
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORL: {
                        scriptEng.operands[i] = entityPtr->flailing[0];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORC: {
                        scriptEng.operands[i] = entityPtr->flailing[1];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORR: {
                        scriptEng.operands[i] = entityPtr->flailing[2];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORLC: {
                        scriptEng.operands[i] = entityPtr->flailing[3];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORRC: {
                        scriptEng.operands[i] = entityPtr->flailing[4];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONLEFT: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONTOP: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONRIGHT: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONBOTTOM: {
                        break;
                    }
                    case VAR_OBJECTOUTOFBOUNDS: {
                        break;
                    }
                    case VAR_OBJECTSPRITESHEET: {
                        scriptEng.operands[i] = objectScriptList[entityPtr->type].spriteSheetID;
                        break;
                    }
                    case VAR_OBJECTVALUE0: {
                        scriptEng.operands[i] = entityPtr->values[0];
                        break;
                    }
                    case VAR_OBJECTVALUE1: {
                        scriptEng.operands[i] = entityPtr->values[1];
                        break;
                    }
                    case VAR_OBJECTVALUE2: {
                        scriptEng.operands[i] = entityPtr->values[2];
                        break;
                    }
                    case VAR_OBJECTVALUE3: {
                        scriptEng.operands[i] = entityPtr->values[3];
                        break;
                    }
                    case VAR_OBJECTVALUE4: {
                        scriptEng.operands[i] = entityPtr->values[4];
                        break;
                    }
                    case VAR_OBJECTVALUE5: {
                        scriptEng.operands[i] = entityPtr->values[5];
                        break;
                    }
                    case VAR_OBJECTVALUE6: {
                        scriptEng.operands[i] = entityPtr->values[6];
                        break;
                    }
                    case VAR_OBJECTVALUE7: {
                        scriptEng.operands[i] = entityPtr->values[7];
                        break;
                    }
                    case VAR_OBJECTVALUE8: {
                        scriptEng.operands[i] = entityPtr->values[8];
                        break;
                    }
                    case VAR_OBJECTVALUE9: {
                        scriptEng.operands[i] = entityPtr->values[9];
                        break;
                    }
                    case VAR_OBJECTVALUE10: {
                        scriptEng.operands[i] = entityPtr->values[10];
                        break;
                    }
                    case VAR_OBJECTVALUE11: {
                        scriptEng.operands[i] = entityPtr->values[11];
                        break;
                    }
                    case VAR_OBJECTVALUE12: {
                        scriptEng.operands[i] = entityPtr->values[12];
                        break;
                    }
                    case VAR_OBJECTVALUE13: {
                        scriptEng.operands[i] = entityPtr->values[13];
                        break;
                    }
                    case VAR_OBJECTVALUE14: {
                        scriptEng.operands[i] = entityPtr->values[14];
                        break;
                    }
                    case VAR_OBJECTVALUE15: {
                        scriptEng.operands[i] = entityPtr->values[15];
                        break;
                    }
                    case VAR_OBJECTVALUE16: {
                        scriptEng.operands[i] = entityPtr->values[16];
                        break;
                    }
                    case VAR_OBJECTVALUE17: {
                        scriptEng.operands[i] = entityPtr->values[17];
                        break;
                    }
                    case VAR_OBJECTVALUE18: {
                        scriptEng.operands[i] = entityPtr->values[18];
                        break;
                    }
                    case VAR_OBJECTVALUE19: {
                        scriptEng.operands[i] = entityPtr->values[19];
                        break;
                    }
                    case VAR_OBJECTVALUE20: {
                        scriptEng.operands[i] = entityPtr->values[20];
                        break;
                    }
                    case VAR_OBJECTVALUE21: {
                        scriptEng.operands[i] = entityPtr->values[21];
                        break;
                    }
                    case VAR_OBJECTVALUE22: {
                        scriptEng.operands[i] = entityPtr->values[22];
                        break;
                    }
                    case VAR_OBJECTVALUE23: {
                        scriptEng.operands[i] = entityPtr->values[23];
                        break;
                    }
                    case VAR_OBJECTVALUE24: {
                        scriptEng.operands[i] = entityPtr->values[24];
                        break;
                    }
                    case VAR_OBJECTVALUE25: {
                        scriptEng.operands[i] = entityPtr->values[25];
                        break;
                    }
                    case VAR_OBJECTVALUE26: {
                        scriptEng.operands[i] = entityPtr->values[26];
                        break;
                    }
                    case VAR_OBJECTVALUE27: {
                        scriptEng.operands[i] = entityPtr->values[27];
                        break;
                    }
                    case VAR_OBJECTVALUE28: {
                        scriptEng.operands[i] = entityPtr->values[28];
                        break;
                    }
                    case VAR_OBJECTVALUE29: {
                        scriptEng.operands[i] = entityPtr->values[29];
                        break;
                    }
                    case VAR_OBJECTVALUE30: {
                        scriptEng.operands[i] = entityPtr->values[30];
                        break;
                    }
                    case VAR_OBJECTVALUE31: {
                        scriptEng.operands[i] = entityPtr->values[31];
                        break;
                    }
                    case VAR_OBJECTVALUE32: {
                        scriptEng.operands[i] = entityPtr->values[32];
                        break;
                    }
                    case VAR_OBJECTVALUE33: {
                        scriptEng.operands[i] = entityPtr->values[33];
                        break;
                    }
                    case VAR_OBJECTVALUE34: {
                        scriptEng.operands[i] = entityPtr->values[34];
                        break;
                    }
                    case VAR_OBJECTVALUE35: {
                        scriptEng.operands[i] = entityPtr->values[35];
                        break;
                    }
                    case VAR_OBJECTVALUE36: {
                        scriptEng.operands[i] = entityPtr->values[36];
                        break;
                    }
                    case VAR_OBJECTVALUE37: {
                        scriptEng.operands[i] = entityPtr->values[37];
                        break;
                    }
                    case VAR_OBJECTVALUE38: {
                        scriptEng.operands[i] = entityPtr->values[38];
                        break;
                    }
                    case VAR_OBJECTVALUE39: {
                        scriptEng.operands[i] = entityPtr->values[39];
                        break;
                    }
                    case VAR_OBJECTVALUE40: {
                        scriptEng.operands[i] = entityPtr->values[40];
                        break;
                    }
                    case VAR_OBJECTVALUE41: {
                        scriptEng.operands[i] = entityPtr->values[41];
                        break;
                    }
                    case VAR_OBJECTVALUE42: {
                        scriptEng.operands[i] = entityPtr->values[42];
                        break;
                    }
                    case VAR_OBJECTVALUE43: {
                        scriptEng.operands[i] = entityPtr->values[43];
                        break;
                    }
                    case VAR_OBJECTVALUE44: {
                        scriptEng.operands[i] = entityPtr->values[44];
                        break;
                    }
                    case VAR_OBJECTVALUE45: {
                        scriptEng.operands[i] = entityPtr->values[45];
                        break;
                    }
                    case VAR_OBJECTVALUE46: {
                        scriptEng.operands[i] = entityPtr->values[46];
                        break;
                    }
                    case VAR_OBJECTVALUE47: {
                        scriptEng.operands[i] = entityPtr->values[47];
                        break;
                    }
                    case VAR_STAGESTATE: break;
                    case VAR_STAGEACTIVELIST: break;
                    case VAR_STAGELISTPOS: break;
                    case VAR_STAGETIMEENABLED: break;
                    case VAR_STAGEMILLISECONDS: break;
                    case VAR_STAGESECONDS: break;
                    case VAR_STAGEMINUTES: break;
                    case VAR_STAGEACTNUM: break;
                    case VAR_STAGEPAUSEENABLED: break;
                    case VAR_STAGELISTSIZE: break;
                    case VAR_STAGENEWXBOUNDARY1: break;
                    case VAR_STAGENEWXBOUNDARY2: break;
                    case VAR_STAGENEWYBOUNDARY1: break;
                    case VAR_STAGENEWYBOUNDARY2: break;
                    case VAR_STAGECURXBOUNDARY1: break;
                    case VAR_STAGECURXBOUNDARY2: break;
                    case VAR_STAGECURYBOUNDARY1: break;
                    case VAR_STAGECURYBOUNDARY2: break;
                    case VAR_STAGEDEFORMATIONDATA0: break;
                    case VAR_STAGEDEFORMATIONDATA1: break;
                    case VAR_STAGEDEFORMATIONDATA2: break;
                    case VAR_STAGEDEFORMATIONDATA3: break;
                    case VAR_STAGEWATERLEVEL: break;
                    case VAR_STAGEACTIVELAYER: break;
                    case VAR_STAGEMIDPOINT: break;
                    case VAR_STAGEPLAYERLISTPOS: break;
                    case VAR_STAGEDEBUGMODE: break;
                    case VAR_STAGEENTITYPOS: break;
                    case VAR_SCREENCAMERAENABLED: break;
                    case VAR_SCREENCAMERATARGET: break;
                    case VAR_SCREENCAMERASTYLE: break;
                    case VAR_SCREENCAMERAX: break;
                    case VAR_SCREENCAMERAY: break;
                    case VAR_SCREENDRAWLISTSIZE: break;
                    case VAR_SCREENXCENTER: break;
                    case VAR_SCREENYCENTER: break;
                    case VAR_SCREENXSIZE: break;
                    case VAR_SCREENYSIZE: break;
                    case VAR_SCREENXOFFSET: break;
                    case VAR_SCREENYOFFSET: break;
                    case VAR_SCREENSHAKEX: break;
                    case VAR_SCREENSHAKEY: break;
                    case VAR_SCREENADJUSTCAMERAY: break;
                    case VAR_TOUCHSCREENDOWN: break;
                    case VAR_TOUCHSCREENXPOS: break;
                    case VAR_TOUCHSCREENYPOS: break;
                    case VAR_MUSICVOLUME: break;
                    case VAR_MUSICCURRENTTRACK: break;
                    case VAR_MUSICPOSITION: break;
                    case VAR_KEYDOWNUP: break;
                    case VAR_KEYDOWNDOWN: break;
                    case VAR_KEYDOWNLEFT: break;
                    case VAR_KEYDOWNRIGHT: break;
                    case VAR_KEYDOWNBUTTONA: break;
                    case VAR_KEYDOWNBUTTONB: break;
                    case VAR_KEYDOWNBUTTONC: break;
                    case VAR_KEYDOWNBUTTONX: break;
                    case VAR_KEYDOWNBUTTONY: break;
                    case VAR_KEYDOWNBUTTONZ: break;
                    case VAR_KEYDOWNBUTTONL: break;
                    case VAR_KEYDOWNBUTTONR: break;
                    case VAR_KEYDOWNSTART: break;
                    case VAR_KEYDOWNSELECT: break;
                    case VAR_KEYPRESSUP: break;
                    case VAR_KEYPRESSDOWN: break;
                    case VAR_KEYPRESSLEFT: break;
                    case VAR_KEYPRESSRIGHT: break;
                    case VAR_KEYPRESSBUTTONA: break;
                    case VAR_KEYPRESSBUTTONB: break;
                    case VAR_KEYPRESSBUTTONC: break;
                    case VAR_KEYPRESSBUTTONX: break;
                    case VAR_KEYPRESSBUTTONY: break;
                    case VAR_KEYPRESSBUTTONZ: break;
                    case VAR_KEYPRESSBUTTONL: break;
                    case VAR_KEYPRESSBUTTONR: break;
                    case VAR_KEYPRESSSTART: break;
                    case VAR_KEYPRESSSELECT: break;
                    case VAR_MENU1SELECTION: break;
                    case VAR_MENU2SELECTION: break;
                    case VAR_TILELAYERXSIZE: break;
                    case VAR_TILELAYERYSIZE: break;
                    case VAR_TILELAYERTYPE: break;
                    case VAR_TILELAYERANGLE: break;
                    case VAR_TILELAYERXPOS: break;
                    case VAR_TILELAYERYPOS: break;
                    case VAR_TILELAYERZPOS: break;
                    case VAR_TILELAYERPARALLAXFACTOR: break;
                    case VAR_TILELAYERSCROLLSPEED: break;
                    case VAR_TILELAYERSCROLLPOS: break;
                    case VAR_TILELAYERDEFORMATIONOFFSET: break;
                    case VAR_TILELAYERDEFORMATIONOFFSETW: break;
                    case VAR_HPARALLAXPARALLAXFACTOR: break;
                    case VAR_HPARALLAXSCROLLSPEED: break;
                    case VAR_HPARALLAXSCROLLPOS: break;
                    case VAR_VPARALLAXPARALLAXFACTOR: break;
                    case VAR_VPARALLAXSCROLLSPEED: break;
                    case VAR_VPARALLAXSCROLLPOS: break;
                    case VAR_SCENE3DVERTEXCOUNT: break;
                    case VAR_SCENE3DFACECOUNT: break;
                    case VAR_SCENE3DPROJECTIONX: break;
                    case VAR_SCENE3DPROJECTIONY: break;
                    case VAR_SCENE3DFOGCOLOR: break;
                    case VAR_SCENE3DFOGSTRENGTH: break;
                    case VAR_VERTEXBUFFERX: break;
                    case VAR_VERTEXBUFFERY: break;
                    case VAR_VERTEXBUFFERZ: break;
                    case VAR_VERTEXBUFFERU: break;
                    case VAR_VERTEXBUFFERV: break;
                    case VAR_FACEBUFFERA: break;
                    case VAR_FACEBUFFERB: break;
                    case VAR_FACEBUFFERC: break;
                    case VAR_FACEBUFFERD: break;
                    case VAR_FACEBUFFERFLAG: break;
                    case VAR_FACEBUFFERCOLOR: break;
                    case VAR_SAVERAM: break;
                    case VAR_ENGINESTATE: break;
                    case VAR_ENGINELANGUAGE: break;
                    case VAR_ENGINEONLINEACTIVE: break;
                    case VAR_ENGINESFXVOLUME: break;
                    case VAR_ENGINEBGMVOLUME: break;
                    case VAR_ENGINETRIALMODE: break;
                    case VAR_ENGINEDEVICETYPE:
                        break;
                        // EDITOR-ONLY
                    case VAR_EDITORVARIABLEID:
                        scriptEng.operands[i] = scnEditor->viewer->variableID;
                        break;
                    case VAR_EDITORVARIABLEVAL:
                        scriptEng.operands[i] = scnEditor->viewer->variableValue;
                        break;
                    case VAR_EDITORRETURNVAR:
                        scriptEng.operands[i] = scnEditor->viewer->returnVariable;
                        break;
                    case VAR_EDITORSHOWGIZMOS:
                        scriptEng.operands[i] = scnEditor->viewer->sceneInfo.effectGizmo
                                                || scnEditor->viewer->selectedEntity == objectEntityPos;
                        break;
                    case VAR_EDITORDRAWINGOVERLAY:
                        scriptEng.operands[i] = scnEditor->viewer->sceneInfo.debugMode;
                        break;
                }
            }
            else if (opcodeType == SCRIPTVAR_INTCONST) { // int constant
                scriptEng.operands[i] = scriptCode[scriptCodePtr++];
            }
            else if (opcodeType == SCRIPTVAR_STRCONST) { // string constant
                int strLen         = scriptCode[scriptCodePtr++];
                scriptText[strLen] = 0;
                for (int c = 0; c < strLen; ++c) {
                    switch (c % 4) {
                        case 0: scriptText[c] = scriptCode[scriptCodePtr] >> 24; break;

                        case 1: scriptText[c] = (0xFFFFFF & scriptCode[scriptCodePtr]) >> 16; break;

                        case 2: scriptText[c] = (0xFFFF & scriptCode[scriptCodePtr]) >> 8; break;

                        case 3: scriptText[c] = scriptCode[scriptCodePtr++]; break;

                        default: break;
                    }
                }
                scriptCodePtr++;
            }
        }

        ObjectScript *scriptInfo = &objectScriptList[objectEntityList[objectEntityPos].type];
        Entity *entity           = &objectEntityList[objectEntityPos];
        SpriteFrame *spriteFrame = nullptr;

        // Functions
        switch (opcode) {
            default: break;
            case FUNC_END: running = false; break;
            case FUNC_EQUAL: scriptEng.operands[0] = scriptEng.operands[1]; break;
            case FUNC_ADD: scriptEng.operands[0] += scriptEng.operands[1]; break;
            case FUNC_SUB: scriptEng.operands[0] -= scriptEng.operands[1]; break;
            case FUNC_INC: ++scriptEng.operands[0]; break;
            case FUNC_DEC: --scriptEng.operands[0]; break;
            case FUNC_MUL: scriptEng.operands[0] *= scriptEng.operands[1]; break;
            case FUNC_DIV: scriptEng.operands[0] /= scriptEng.operands[1]; break;
            case FUNC_SHR: scriptEng.operands[0] >>= scriptEng.operands[1]; break;
            case FUNC_SHL: scriptEng.operands[0] <<= scriptEng.operands[1]; break;
            case FUNC_AND: scriptEng.operands[0] &= scriptEng.operands[1]; break;
            case FUNC_OR: scriptEng.operands[0] |= scriptEng.operands[1]; break;
            case FUNC_XOR: scriptEng.operands[0] ^= scriptEng.operands[1]; break;
            case FUNC_MOD: scriptEng.operands[0] %= scriptEng.operands[1]; break;
            case FUNC_FLIPSIGN: scriptEng.operands[0] = -scriptEng.operands[0]; break;
            case FUNC_CHECKEQUAL:
                scriptEng.checkResult = scriptEng.operands[0] == scriptEng.operands[1];
                opcodeSize            = 0;
                break;
            case FUNC_CHECKGREATER:
                scriptEng.checkResult = scriptEng.operands[0] > scriptEng.operands[1];
                opcodeSize            = 0;
                break;
            case FUNC_CHECKLOWER:
                scriptEng.checkResult = scriptEng.operands[0] < scriptEng.operands[1];
                opcodeSize            = 0;
                break;
            case FUNC_CHECKNOTEQUAL:
                scriptEng.checkResult = scriptEng.operands[0] != scriptEng.operands[1];
                opcodeSize            = 0;
                break;
            case FUNC_IFEQUAL:
                if (scriptEng.operands[1] != scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFGREATER:
                if (scriptEng.operands[1] <= scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFGREATEROREQUAL:
                if (scriptEng.operands[1] < scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[+jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                         = 0;
                break;
            case FUNC_IFLOWER:
                if (scriptEng.operands[1] >= scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFLOWEROREQUAL:
                if (scriptEng.operands[1] > scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFNOTEQUAL:
                if (scriptEng.operands[1] == scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_ELSE:
                opcodeSize    = 0;
                scriptCodePtr = scriptCodeStart
                                + jumpTable[jumpTableStart + jumpTableStack[jumpTableStackPos--] + 1];
                break;
            case FUNC_ENDIF:
                opcodeSize = 0;
                --jumpTableStackPos;
                break;
            case FUNC_WEQUAL:
                if (scriptEng.operands[1] != scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WGREATER:
                if (scriptEng.operands[1] <= scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WGREATEROREQUAL:
                if (scriptEng.operands[1] < scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WLOWER:
                if (scriptEng.operands[1] >= scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WLOWEROREQUAL:
                if (scriptEng.operands[1] > scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WNOTEQUAL:
                if (scriptEng.operands[1] == scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_LOOP:
                opcodeSize = 0;
                scriptCodePtr =
                    scriptCodeStart + jumpTable[jumpTableStart + jumpTableStack[jumpTableStackPos--]];
                break;
            case FUNC_FOREACHACTIVE: {
                int typeGroup = scriptEng.operands[1];
                if (typeGroup < TYPEGROUP_COUNT_v4) {
                    int loop                      = foreachStack[++foreachStackPos] + 1;
                    foreachStack[foreachStackPos] = loop;
                    if (loop >= objectTypeGroupList[typeGroup].listSize) {
                        opcodeSize                      = 0;
                        foreachStack[foreachStackPos--] = -1;
                        scriptCodePtr =
                            scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                        break;
                    }
                    else {
                        scriptEng.operands[2] = objectTypeGroupList[typeGroup].entityRefs[loop];
                        jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                    }
                }
                else {
                    opcodeSize = 0;
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                }
                break;
            }
            case FUNC_FOREACHALL: {
                int objType = scriptEng.operands[1];
                if (objType < OBJECT_COUNT_v4) {
                    int loop                      = foreachStack[++foreachStackPos] + 1;
                    foreachStack[foreachStackPos] = loop;

                    if (scriptEvent == EVENT_RSDKLOAD) {
                        while (true) {
                            if (loop >= TEMPENTITY_START_v4) {
                                opcodeSize                      = 0;
                                foreachStack[foreachStackPos--] = -1;
                                int off       = jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                                scriptCodePtr = scriptCodeStart + off;
                                break;
                            }
                            else if (objType == objectEntityList[loop].type) {
                                scriptEng.operands[2]               = loop;
                                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                                break;
                            }
                            else {
                                foreachStack[foreachStackPos] = ++loop;
                            }
                        }
                    }
                    else {
                        while (true) {
                            if (loop >= ENTITY_COUNT_v4) {
                                opcodeSize                      = 0;
                                foreachStack[foreachStackPos--] = -1;
                                scriptCodePtr                   = scriptCodeStart
                                                + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                                break;
                            }
                            else if (objType == objectEntityList[loop].type) {
                                scriptEng.operands[2]               = loop;
                                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                                break;
                            }
                            else {
                                foreachStack[foreachStackPos] = ++loop;
                            }
                        }
                    }
                }
                else {
                    opcodeSize = 0;
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                }
                break;
            }
            case FUNC_NEXT:
                opcodeSize = 0;
                scriptCodePtr =
                    scriptCodeStart + jumpTable[jumpTableStart + jumpTableStack[jumpTableStackPos--]];
                --foreachStackPos;
                break;
            case FUNC_SWITCH:
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                if (scriptEng.operands[1] < jumpTable[jumpTableStart + scriptEng.operands[0]]
                    || scriptEng.operands[1] > jumpTable[jumpTableStart + scriptEng.operands[0] + 1])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 2];
                else
                    scriptCodePtr =
                        scriptCodeStart
                        + jumpTable[jumpTableStart + scriptEng.operands[0] + 4
                                    + (scriptEng.operands[1]
                                       - jumpTable[jumpTableStart + scriptEng.operands[0]])];
                opcodeSize = 0;
                break;
            case FUNC_BREAK:
                opcodeSize    = 0;
                scriptCodePtr = scriptCodeStart
                                + jumpTable[jumpTableStart + jumpTableStack[jumpTableStackPos--] + 3];
                break;
            case FUNC_ENDSWITCH:
                opcodeSize = 0;
                --jumpTableStackPos;
                break;
            case FUNC_RAND: scriptEng.operands[0] = rand() % scriptEng.operands[1]; break;
            case FUNC_SIN: {
                scriptEng.operands[0] = sin512(scriptEng.operands[1]);
                break;
            }
            case FUNC_COS: {
                scriptEng.operands[0] = cos512(scriptEng.operands[1]);
                break;
            }
            case FUNC_SIN256: {
                scriptEng.operands[0] = sin256(scriptEng.operands[1]);
                break;
            }
            case FUNC_COS256: {
                scriptEng.operands[0] = cos256(scriptEng.operands[1]);
                break;
            }
            case FUNC_ATAN2: {
                scriptEng.operands[0] = ArcTanLookup(scriptEng.operands[1], scriptEng.operands[2]);
                break;
            }
            case FUNC_INTERPOLATE:
                scriptEng.operands[0] = (scriptEng.operands[2] * (0x100 - scriptEng.operands[3])
                                         + scriptEng.operands[3] * scriptEng.operands[1])
                                        >> 8;
                break;
            case FUNC_INTERPOLATEXY:
                scriptEng.operands[0] = (scriptEng.operands[3] * (0x100 - scriptEng.operands[6]) >> 8)
                                        + ((scriptEng.operands[6] * scriptEng.operands[2]) >> 8);
                scriptEng.operands[1] = (scriptEng.operands[5] * (0x100 - scriptEng.operands[6]) >> 8)
                                        + (scriptEng.operands[6] * scriptEng.operands[4] >> 8);
                break;
            case FUNC_LOADSPRITESHEET:
                opcodeSize                = 0;
                scriptInfo->spriteSheetID = editor->loadSpriteSheet(scriptText);
                break;
            case FUNC_REMOVESPRITESHEET:
                opcodeSize = 0;
                editor->viewer->removeGraphicsFile(scriptText, -1);
                break;
            case FUNC_DRAWSPRITE:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->drawSpriteFlipped((entity->XPos >> 16) + spriteFrame->pivotX,
                                          (entity->YPos >> 16) + spriteFrame->pivotY,
                                          spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                          spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                          scriptInfo->spriteSheetID, false);
                break;
            case FUNC_DRAWSPRITEXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->drawSpriteFlipped((scriptEng.operands[1] >> 16) + spriteFrame->pivotX,
                                          (scriptEng.operands[2] >> 16) + spriteFrame->pivotY,
                                          spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                          spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                          scriptInfo->spriteSheetID, false);
                break;
            case FUNC_DRAWSPRITESCREENXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->drawSpriteFlipped(scriptEng.operands[1] + spriteFrame->pivotX,
                                          scriptEng.operands[2] + spriteFrame->pivotY,
                                          spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                          spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                          scriptInfo->spriteSheetID, true);
                break;
            case FUNC_DRAWTINTRECT: opcodeSize = 0; break;
            case FUNC_DRAWNUMBERS: {
                opcodeSize = 0;
                int i      = 10;
                if (scriptEng.operands[6]) {
                    while (scriptEng.operands[4] > 0) {
                        int frameID = scriptEng.operands[3] % i / (i / 10) + scriptEng.operands[0];
                        spriteFrame = &scriptFrames[scriptInfo->frameListOffset + frameID];
                        editor->drawSpriteFlipped(spriteFrame->pivotX + scriptEng.operands[1],
                                                  spriteFrame->pivotY + scriptEng.operands[2],
                                                  spriteFrame->width, spriteFrame->height,
                                                  spriteFrame->sprX, spriteFrame->sprY, FLIP_NONE,
                                                  INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        scriptEng.operands[1] -= scriptEng.operands[5];
                        i *= 10;
                        --scriptEng.operands[4];
                    }
                }
                else {
                    int extra = 10;
                    if (scriptEng.operands[3])
                        extra = 10 * scriptEng.operands[3];
                    while (scriptEng.operands[4] > 0) {
                        if (extra >= i) {
                            int frameID = scriptEng.operands[3] % i / (i / 10) + scriptEng.operands[0];
                            spriteFrame = &scriptFrames[scriptInfo->frameListOffset + frameID];
                            editor->drawSpriteFlipped(spriteFrame->pivotX + scriptEng.operands[1],
                                                      spriteFrame->pivotY + scriptEng.operands[2],
                                                      spriteFrame->width, spriteFrame->height,
                                                      spriteFrame->sprX, spriteFrame->sprY, FLIP_NONE,
                                                      INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        }
                        scriptEng.operands[1] -= scriptEng.operands[5];
                        i *= 10;
                        --scriptEng.operands[4];
                    }
                }
                break;
            }
            case FUNC_DRAWACTNAME: {
                opcodeSize = 0;
                break;
            }
            case FUNC_DRAWMENU: opcodeSize = 0; break;
            case FUNC_SPRITEFRAME:
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD && scriptFrameCount < SPRITEFRAME_COUNT_v4) {
                    scriptFrames[scriptFrameCount].pivotX = scriptEng.operands[0];
                    scriptFrames[scriptFrameCount].pivotY = scriptEng.operands[1];
                    scriptFrames[scriptFrameCount].width  = scriptEng.operands[2];
                    scriptFrames[scriptFrameCount].height = scriptEng.operands[3];
                    scriptFrames[scriptFrameCount].sprX   = scriptEng.operands[4];
                    scriptFrames[scriptFrameCount].sprY   = scriptEng.operands[5];
                    ++scriptFrameCount;
                }
                break;
            case FUNC_EDITFRAME: {
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];

                spriteFrame->pivotX = scriptEng.operands[1];
                spriteFrame->pivotY = scriptEng.operands[2];
                spriteFrame->width  = scriptEng.operands[3];
                spriteFrame->height = scriptEng.operands[4];
                spriteFrame->sprX   = scriptEng.operands[5];
                spriteFrame->sprY   = scriptEng.operands[6];
                break;
            }
            case FUNC_LOADPALETTE: opcodeSize = 0; break;
            case FUNC_ROTATEPALETTE: opcodeSize = 0; break;
            case FUNC_SETSCREENFADE: opcodeSize = 0; break;
            case FUNC_SETACTIVEPALETTE: opcodeSize = 0; break;
            case FUNC_SETPALETTEFADE: break;
            case FUNC_SETPALETTEENTRY: opcodeSize = 0; break;
            case FUNC_GETPALETTEENTRY: opcodeSize = 0; break;
            case FUNC_COPYPALETTE: opcodeSize = 0; break;
            case FUNC_CLEARSCREEN: opcodeSize = 0; break;
            case FUNC_DRAWSPRITEFX:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                switch (scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        editor->drawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16), (scriptEng.operands[3] >> 16),
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, entity->scale,
                            entity->scale, entity->direction, 0, INK_NONE, 0xFF,
                            scriptInfo->spriteSheetID, false);
                        break;
                    case FX_ROTATE:
                        editor->drawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16), (scriptEng.operands[3] >> 16),
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, 0x200, 0x200,
                            entity->direction, entity->rotation, INK_NONE, 0xFF,
                            scriptInfo->spriteSheetID, false);
                        break;
                    case FX_ROTOZOOM:
                        editor->drawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16), (scriptEng.operands[3] >> 16),
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, entity->scale,
                            entity->scale, entity->direction, entity->rotation, INK_NONE, 0xFF,
                            scriptInfo->spriteSheetID, false);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_BLEND:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_BLEND, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_ALPHA:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_ALPHA, entity->alpha,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_ADD:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_ADD, entity->alpha,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_SUB:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_SUB, entity->alpha,
                                    scriptInfo->spriteSheetID, false);
                                break;
                        }
                        break;
                    case FX_TINT: break;
                    case FX_FLIP:
                        switch (entity->direction) {
                            default:
                            case FLIP_NONE:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case FLIP_X:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) - spriteFrame->width
                                        - spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_X, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case FLIP_Y:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) - spriteFrame->height
                                        - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_Y, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case FLIP_XY:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) - spriteFrame->width
                                        - spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) - spriteFrame->height
                                        - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_XY, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                        }
                        break;
                }
                break;
            case FUNC_DRAWSPRITESCREENFX:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                switch (scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        editor->drawSpriteRotozoom(
                            scriptEng.operands[2], scriptEng.operands[3], -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, entity->scale, entity->scale,
                            entity->direction, 0, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        break;
                    case FX_ROTATE:
                        editor->drawSpriteRotozoom(
                            scriptEng.operands[2], scriptEng.operands[3], -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, 0x200, 0x200, entity->direction,
                            entity->rotation, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        break;
                    case FX_ROTOZOOM:
                        editor->drawSpriteRotozoom(scriptEng.operands[2], scriptEng.operands[3],
                                                   -spriteFrame->pivotX, -spriteFrame->pivotY,
                                                   spriteFrame->width, spriteFrame->height,
                                                   spriteFrame->sprX, spriteFrame->sprY, entity->scale,
                                                   entity->scale, entity->direction, entity->rotation,
                                                   INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case INK_BLEND:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_BLEND, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case INK_ALPHA:
                                editor->drawSpriteFlipped(scriptEng.operands[2] + spriteFrame->pivotX,
                                                          scriptEng.operands[3] + spriteFrame->pivotY,
                                                          spriteFrame->width, spriteFrame->height,
                                                          spriteFrame->sprX, spriteFrame->sprY,
                                                          FLIP_NONE, INK_ALPHA, entity->alpha,
                                                          scriptInfo->spriteSheetID, true);
                                break;
                            case INK_ADD:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_ADD, entity->alpha, scriptInfo->spriteSheetID, true);
                                break;
                            case INK_SUB:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_SUB, entity->alpha, scriptInfo->spriteSheetID, true);
                                break;
                        }
                        break;
                    case FX_TINT: break;
                    case FX_FLIP:
                        switch (entity->direction) {
                            default:
                            case FLIP_NONE:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case FLIP_X:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_X,
                                    INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case FLIP_Y:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_Y,
                                    INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case FLIP_XY:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_XY,
                                    INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                        }
                        break;
                }
                break;
            case FUNC_LOADANIMATION: opcodeSize = 0; break;
            case FUNC_SETUPMENU: {
                opcodeSize = 0;
                break;
            }
            case FUNC_ADDMENUENTRY: {
                opcodeSize = 0;
                break;
            }
            case FUNC_EDITMENUENTRY: {
                opcodeSize = 0;
                break;
            }
            case FUNC_LOADSTAGE: opcodeSize = 0; break;
            case FUNC_DRAWRECT:
                opcodeSize = 0;
                editor->viewer->drawRect(
                    scriptEng.operands[0], scriptEng.operands[1], scriptEng.operands[2],
                    scriptEng.operands[3],
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, scriptEng.operands[7] / 255.0f),
                    false, 0xFF, INK_NONE);
                break;
            case FUNC_RESETOBJECTENTITY: {
                opcodeSize = 0;
                break;
            }
            case FUNC_BOXCOLLISIONTEST: opcodeSize = 0; break;
            case FUNC_CREATETEMPOBJECT: {
                opcodeSize = 0;
                break;
            }
            case FUNC_PROCESSOBJECTMOVEMENT: opcodeSize = 0; break;
            case FUNC_PROCESSOBJECTCONTROL: opcodeSize = 0; break;
            case FUNC_PROCESSANIMATION: opcodeSize = 0; break;
            case FUNC_DRAWOBJECTANIMATION: opcodeSize = 0; break;
            case FUNC_SETMUSICTRACK: opcodeSize = 0; break;
            case FUNC_PLAYMUSIC: opcodeSize = 0; break;
            case FUNC_STOPMUSIC: opcodeSize = 0; break;
            case FUNC_PAUSEMUSIC: opcodeSize = 0; break;
            case FUNC_RESUMEMUSIC: opcodeSize = 0; break;
            case FUNC_SWAPMUSICTRACK: opcodeSize = 0; break;
            case FUNC_PLAYSFX: opcodeSize = 0; break;
            case FUNC_STOPSFX: opcodeSize = 0; break;
            case FUNC_SETSFXATTRIBUTES: opcodeSize = 0; break;
            case FUNC_OBJECTTILECOLLISION: opcodeSize = 0; break;
            case FUNC_OBJECTTILEGRIP: opcodeSize = 0; break;
            case FUNC_NOT: scriptEng.operands[0] = ~scriptEng.operands[0]; break;
            case FUNC_DRAW3DSCENE: opcodeSize = 0; break;
            case FUNC_SETIDENTITYMATRIX: opcodeSize = 0; break;
            case FUNC_MATRIXMULTIPLY: opcodeSize = 0; break;
            case FUNC_MATRIXTRANSLATEXYZ: opcodeSize = 0; break;
            case FUNC_MATRIXSCALEXYZ: opcodeSize = 0; break;
            case FUNC_MATRIXROTATEX: opcodeSize = 0; break;
            case FUNC_MATRIXROTATEY: opcodeSize = 0; break;
            case FUNC_MATRIXROTATEZ: opcodeSize = 0; break;
            case FUNC_MATRIXROTATEXYZ: opcodeSize = 0; break;
            case FUNC_MATRIXINVERSE: opcodeSize = 0; break;
            case FUNC_TRANSFORMVERTICES: opcodeSize = 0; break;
            case FUNC_CALLFUNCTION:
                opcodeSize                        = 0;
                functionStack[functionStackPos++] = scriptCodePtr;
                functionStack[functionStackPos++] = jumpTableStart;
                functionStack[functionStackPos++] = scriptCodeStart;
                scriptCodeStart = functionList[scriptEng.operands[0]].ptr.scriptCodePtr;
                jumpTableStart  = functionList[scriptEng.operands[0]].ptr.jumpTablePtr;
                scriptCodePtr   = scriptCodeStart;
                break;
            case FUNC_RETURN:
                opcodeSize = 0;
                if (!functionStackPos) { // event, stop running
                    running = false;
                }
                else { // function, jump out
                    scriptCodeStart = functionStack[--functionStackPos];
                    jumpTableStart  = functionStack[--functionStackPos];
                    scriptCodePtr   = functionStack[--functionStackPos];
                }
                break;
            case FUNC_SETLAYERDEFORMATION: opcodeSize = 0; break;
            case FUNC_CHECKTOUCHRECT: opcodeSize = 0; break;
            case FUNC_GETTILELAYERENTRY: opcodeSize = 0; break;
            case FUNC_SETTILELAYERENTRY: opcodeSize = 0; break;
            case FUNC_GETBIT:
                scriptEng.operands[0] =
                    (scriptEng.operands[1] & (1 << scriptEng.operands[2])) >> scriptEng.operands[2];
                break;
            case FUNC_SETBIT:
                if (scriptEng.operands[2] <= 0)
                    scriptEng.operands[0] &= ~(1 << scriptEng.operands[1]);
                else
                    scriptEng.operands[0] |= 1 << scriptEng.operands[1];
                break;
            case FUNC_CLEARDRAWLIST: opcodeSize = 0; break;
            case FUNC_ADDDRAWLISTENTITYREF: {
                opcodeSize = 0;
                // drawListEntries[scriptEng.operands[0]].entityRefs[drawListEntries[scriptEng.operands[0]].listSize++]
                // = scriptEng.operands[1];
                break;
            }
            case FUNC_GETDRAWLISTENTITYREF:
                // scriptEng.operands[0] =
                // drawListEntries[scriptEng.operands[1]].entityRefs[scriptEng.operands[2]];
                break;
            case FUNC_SETDRAWLISTENTITYREF:
                opcodeSize = 0;
                // drawListEntries[scriptEng.operands[1]].entityRefs[scriptEng.operands[2]] =
                // scriptEng.operands[0];
                break;
            case FUNC_GET16X16TILEINFO: {
                opcodeSize = 0;
                break;
            }
            case FUNC_SET16X16TILEINFO: {
                opcodeSize = 0;
                break;
            }
            case FUNC_COPY16X16TILE: opcodeSize = 0; break;
            case FUNC_GETANIMATIONBYNAME: {
                opcodeSize = 0;
                break;
            }
            case FUNC_READSAVERAM: opcodeSize = 0; break;
            case FUNC_WRITESAVERAM: opcodeSize = 0; break;
            case FUNC_LOADTEXTFILE: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETTEXTINFO: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETVERSIONNUMBER: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETTABLEVALUE: {
                int arrPos = scriptEng.operands[1];
                if (arrPos >= 0) {
                    int pos     = scriptEng.operands[2];
                    int arrSize = scriptCode[pos];
                    if (arrPos < arrSize)
                        scriptEng.operands[0] = scriptCode[pos + arrPos + 1];
                }
                break;
            }
            case FUNC_SETTABLEVALUE: {
                opcodeSize = 0;
                int arrPos = scriptEng.operands[1];
                if (arrPos >= 0) {
                    int pos     = scriptEng.operands[2];
                    int arrSize = scriptCode[pos];
                    if (arrPos < arrSize)
                        scriptCode[pos + arrPos + 1] = scriptEng.operands[0];
                }
                break;
            }
            case FUNC_CHECKCURRENTSTAGEFOLDER:
                opcodeSize            = 0;
                scriptEng.checkResult = editor->viewer->currentFolder == scriptText;

                if (!scriptEng.checkResult) {
                    int targetSize  = editor->viewer->currentFolder.size();
                    int currentSize = strlen(scriptText);
                    if (targetSize > currentSize) {
                        scriptEng.checkResult = editor->viewer->currentFolder.endsWith(scriptText);
                    }
                }
                break;
            case FUNC_ABS: {
                scriptEng.operands[0] = abs(scriptEng.operands[0]);
                break;
            }
            case FUNC_CALLNATIVEFUNCTION: opcodeSize = 0; break;
            case FUNC_CALLNATIVEFUNCTION2: opcodeSize = 0; break;
            case FUNC_CALLNATIVEFUNCTION4: opcodeSize = 0; break;
            case FUNC_SETOBJECTRANGE: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETOBJECTVALUE: {
                int valID = scriptEng.operands[1];
                if (valID <= 47)
                    scriptEng.operands[0] = objectEntityList[scriptEng.operands[2]].values[valID];
                break;
            }
            case FUNC_SETOBJECTVALUE: {
                opcodeSize = 0;
                int valID  = scriptEng.operands[1];
                if (valID <= 47)
                    objectEntityList[scriptEng.operands[2]].values[valID] = scriptEng.operands[0];
                break;
            }
            case FUNC_COPYOBJECT: {
                opcodeSize = 0;
                // start index, copy offset, count
                Entity *src = &objectEntityList[scriptEng.operands[0]];
                for (int e = 0; e < scriptEng.operands[2]; ++e) {
                    Entity *dst = &src[scriptEng.operands[1]];
                    ++src;
                    memcpy(dst, src, sizeof(Entity));
                }
                break;
            }
            case FUNC_PRINT: {
                if (scriptEng.operands[1])
                    PrintLog(QString::number(scriptEng.operands[0]));
                else
                    PrintLog(scriptText);

                if (scriptEng.operands[2])
                    PrintLog("\n");
                break;
            }
            case FUNC_ADDEDITORVAR: {
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD) {
                    editor->viewer->addEditorVariable(scriptText);
                }
                break;
            }
            case FUNC_SETACTIVEVAR: {
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD) {
                    editor->viewer->setActiveVariable(scriptText);
                }
                break;
            }
            case FUNC_ADDENUMVAR: {
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD) {
                    editor->viewer->addEnumVariable(scriptText, scriptEng.operands[1]);
                }
                break;
            }
            case FUNC_SETVARALIAS: {
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD) {
                    editor->viewer->setVariableAlias(scriptEng.operands[0], scriptText);
                }
                break;
            }
            case FUNC_DRAWLINE: {
                opcodeSize = 0;

                Vector4<float> color =
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, 1.0);

                editor->viewer->drawLine((scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x,
                                         (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y,
                                         (scriptEng.operands[2] >> 16) - editor->viewer->cameraPos.x,
                                         (scriptEng.operands[3] >> 16) - editor->viewer->cameraPos.y,
                                         color, entity->alpha, (InkEffects)entity->inkEffect);
                break;
            }
            case FUNC_DRAWARROW: {
                opcodeSize = 0;

                int angle = ArcTanLookup(scriptEng.operands[0] - scriptEng.operands[2],
                                         scriptEng.operands[1] - scriptEng.operands[3]);
                Vector4<float> color =
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, 1.0);
                InkEffects ink = (InkEffects)entity->inkEffect;

                int x1 = (scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x;
                int y1 = (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y;
                int x2 = (scriptEng.operands[2] >> 16) - editor->viewer->cameraPos.x;
                int y2 = (scriptEng.operands[3] >> 16) - editor->viewer->cameraPos.y;

                editor->viewer->drawLine(x1, y1, x2, y2, color, entity->alpha, ink);
                editor->viewer->drawLine(x2, y2, x2 + ((cos256(angle + 12) << 12) >> 16),
                                         y2 + ((sin256(angle + 12) << 12) >> 16), color, entity->alpha,
                                         ink);
                editor->viewer->drawLine(x2, y2, x2 + ((cos256(angle - 12) << 12) >> 16),
                                         y2 + ((sin256(angle - 12) << 12) >> 16), color, entity->alpha,
                                         ink);
                break;
            }
            case FUNC_DRAWRECTWORLD: {
                opcodeSize = 0;
                int x      = (scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x;
                int y      = (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y;

                editor->viewer->drawRect(
                    x, y, scriptEng.operands[2], scriptEng.operands[3],
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, scriptEng.operands[7] / 255.0f),
                    false, 0xFF, INK_NONE);
                break;
            }
            case FUNC_DRAWRECTOUTLINE: {
                opcodeSize = 0;

                int x = (scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x;
                int y = (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y;

                editor->viewer->drawRect(
                    x, y, scriptEng.operands[2], scriptEng.operands[3],
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, scriptEng.operands[7] / 255.0f),
                    true, 0xFF, INK_NONE);
                break;
            }
            case FUNC_GETOBJECTTYPE: {
                char typeName[0x40];
                int objPos  = 0;
                int typePos = 0;
                while (scriptText[objPos]) {
                    if (scriptText[objPos] != ' ')
                        typeName[typePos++] = scriptText[objPos];
                    ++objPos;
                }
                typeName[typePos] = 0;

                scriptEng.operands[0] = -1;
                for (int o = 0; o < OBJECT_COUNT_v4; ++o) {
                    if (StrComp(typeName, typeNames[o])) {
                        scriptEng.operands[0] = o;
                    }
                }
                break;
            }
        }

        // Set Values
        if (opcodeSize > 0)
            scriptCodePtr -= scriptCodePtr - scriptCodeOffset;

        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = scriptCode[scriptCodePtr++];
            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (scriptCode[scriptCodePtr++]) { // variable
                    case VARARR_NONE: arrayVal = objectEntityPos; break;

                    case VARARR_ARRAY:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal = scriptEng.arrayPosition[scriptCode[scriptCodePtr++]];
                        else
                            arrayVal = scriptCode[scriptCodePtr++];
                        break;

                    case VARARR_ENTNOPLUS1:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal =
                                objectEntityPos + scriptEng.arrayPosition[scriptCode[scriptCodePtr++]];
                        else
                            arrayVal = objectEntityPos + scriptCode[scriptCodePtr++];
                        break;

                    case VARARR_ENTNOMINUS1:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal =
                                objectEntityPos - scriptEng.arrayPosition[scriptCode[scriptCodePtr++]];
                        else
                            arrayVal = objectEntityPos - scriptCode[scriptCodePtr++];
                        break;

                    default: break;
                }

                // Allows the game to link to the editor properly
                Entity *entityPtr = &objectEntityList[arrayVal];
                if (editor) {
                    for (int e = 0; e < editor->viewer->entities.count(); ++e) {
                        if (arrayVal == editor->viewer->entities[e].slotID) {
                            entityPtr = &objectEntityList[editor->viewer->entities[e].gameEntitySlot];
                        }
                    }
                }

                // Variables
                switch (scriptCode[scriptCodePtr++]) {
                    default: break;
                    case VAR_TEMP0: scriptEng.tempValue[0] = scriptEng.operands[i]; break;
                    case VAR_TEMP1: scriptEng.tempValue[1] = scriptEng.operands[i]; break;
                    case VAR_TEMP2: scriptEng.tempValue[2] = scriptEng.operands[i]; break;
                    case VAR_TEMP3: scriptEng.tempValue[3] = scriptEng.operands[i]; break;
                    case VAR_TEMP4: scriptEng.tempValue[4] = scriptEng.operands[i]; break;
                    case VAR_TEMP5: scriptEng.tempValue[5] = scriptEng.operands[i]; break;
                    case VAR_TEMP6: scriptEng.tempValue[6] = scriptEng.operands[i]; break;
                    case VAR_TEMP7: scriptEng.tempValue[7] = scriptEng.operands[i]; break;
                    case VAR_CHECKRESULT: scriptEng.checkResult = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS0: scriptEng.arrayPosition[0] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS1: scriptEng.arrayPosition[1] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS2: scriptEng.arrayPosition[2] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS3: scriptEng.arrayPosition[3] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS4: scriptEng.arrayPosition[4] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS5: scriptEng.arrayPosition[5] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS6: scriptEng.arrayPosition[6] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS7: scriptEng.arrayPosition[7] = scriptEng.operands[i]; break;
                    case VAR_GLOBAL: /*globalVariables[arrayVal] = scriptEng.operands[i];*/ break;
                    case VAR_LOCAL: scriptCode[arrayVal] = scriptEng.operands[i]; break;
                    case VAR_OBJECTENTITYPOS: objectEntityPos = scriptEng.operands[i]; break;
                    case VAR_OBJECTGROUPID: {
                        entityPtr->typeGroup = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTTYPE: {
                        entityPtr->type = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPROPERTYVALUE: {
                        entityPtr->propertyValue = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTXPOS: {
                        entityPtr->XPos = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTYPOS: {
                        entityPtr->YPos = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTIXPOS: {
                        entityPtr->XPos = scriptEng.operands[i] << 16;
                        break;
                    }
                    case VAR_OBJECTIYPOS: {
                        entityPtr->YPos = scriptEng.operands[i] << 16;
                        break;
                    }
                    case VAR_OBJECTXVEL: {
                        entityPtr->XVelocity = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTYVEL: {
                        entityPtr->YVelocity = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSPEED: {
                        entityPtr->speed = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSTATE: {
                        entityPtr->state = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTROTATION: {
                        entityPtr->rotation = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSCALE: {
                        entityPtr->scale = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPRIORITY: {
                        entityPtr->priority = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDRAWORDER: {
                        entityPtr->drawOrder = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDIRECTION: {
                        entityPtr->direction = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTINKEFFECT: {
                        entityPtr->inkEffect = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTALPHA: {
                        entityPtr->alpha = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFRAME: {
                        entityPtr->frame = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATION: {
                        entityPtr->animation = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPREVANIMATION: {
                        entityPtr->prevAnimation = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATIONSPEED: {
                        entityPtr->animationSpeed = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATIONTIMER: {
                        entityPtr->animationTimer = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANGLE: {
                        entityPtr->angle = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTLOOKPOSX: {
                        entityPtr->lookPosX = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTLOOKPOSY: {
                        entityPtr->lookPosY = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONMODE: {
                        entityPtr->collisionMode = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONPLANE: {
                        entityPtr->collisionPlane = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCONTROLMODE: {
                        entityPtr->controlMode = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCONTROLLOCK: {
                        entityPtr->controlLock = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPUSHING: {
                        entityPtr->pushing = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVISIBLE: {
                        entityPtr->visible = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTTILECOLLISIONS: {
                        entityPtr->tileCollisions = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTINTERACTION: {
                        entityPtr->objectInteractions = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTGRAVITY: {
                        entityPtr->gravity = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTUP: {
                        entityPtr->up = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDOWN: {
                        entityPtr->down = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTLEFT: {
                        entityPtr->left = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTRIGHT: {
                        entityPtr->right = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTJUMPPRESS: {
                        entityPtr->jumpPress = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTJUMPHOLD: {
                        entityPtr->jumpHold = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSCROLLTRACKING: {
                        entityPtr->trackScroll = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORL: {
                        entityPtr->flailing[0] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORC: {
                        entityPtr->flailing[1] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORR: {
                        entityPtr->flailing[2] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORLC: {
                        entityPtr->flailing[3] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORRC: {
                        entityPtr->flailing[4] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONLEFT: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONTOP: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONRIGHT: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONBOTTOM: {
                        break;
                    }
                    case VAR_OBJECTOUTOFBOUNDS: {
                        break;
                    }
                    case VAR_OBJECTSPRITESHEET: {
                        objectScriptList[entityPtr->type].spriteSheetID = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE0: {
                        entityPtr->values[0] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE1: {
                        entityPtr->values[1] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE2: {
                        entityPtr->values[2] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE3: {
                        entityPtr->values[3] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE4: {
                        entityPtr->values[4] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE5: {
                        entityPtr->values[5] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE6: {
                        entityPtr->values[6] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE7: {
                        entityPtr->values[7] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE8: {
                        entityPtr->values[8] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE9: {
                        entityPtr->values[9] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE10: {
                        entityPtr->values[10] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE11: {
                        entityPtr->values[11] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE12: {
                        entityPtr->values[12] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE13: {
                        entityPtr->values[13] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE14: {
                        entityPtr->values[14] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE15: {
                        entityPtr->values[15] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE16: {
                        entityPtr->values[16] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE17: {
                        entityPtr->values[17] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE18: {
                        entityPtr->values[18] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE19: {
                        entityPtr->values[19] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE20: {
                        entityPtr->values[20] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE21: {
                        entityPtr->values[21] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE22: {
                        entityPtr->values[22] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE23: {
                        entityPtr->values[23] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE24: {
                        entityPtr->values[24] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE25: {
                        entityPtr->values[25] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE26: {
                        entityPtr->values[26] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE27: {
                        entityPtr->values[27] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE28: {
                        entityPtr->values[28] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE29: {
                        entityPtr->values[29] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE30: {
                        entityPtr->values[30] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE31: {
                        entityPtr->values[31] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE32: {
                        entityPtr->values[32] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE33: {
                        entityPtr->values[33] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE34: {
                        entityPtr->values[34] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE35: {
                        entityPtr->values[35] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE36: {
                        entityPtr->values[36] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE37: {
                        entityPtr->values[37] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE38: {
                        entityPtr->values[38] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE39: {
                        entityPtr->values[39] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE40: {
                        entityPtr->values[40] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE41: {
                        entityPtr->values[41] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE42: {
                        entityPtr->values[42] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE43: {
                        entityPtr->values[43] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE44: {
                        entityPtr->values[44] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE45: {
                        entityPtr->values[45] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE46: {
                        entityPtr->values[46] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE47: {
                        entityPtr->values[47] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_STAGESTATE: break;
                    case VAR_STAGEACTIVELIST: break;
                    case VAR_STAGELISTPOS: break;
                    case VAR_STAGETIMEENABLED: break;
                    case VAR_STAGEMILLISECONDS: break;
                    case VAR_STAGESECONDS: break;
                    case VAR_STAGEMINUTES: break;
                    case VAR_STAGEACTNUM: break;
                    case VAR_STAGEPAUSEENABLED: break;
                    case VAR_STAGELISTSIZE: break;
                    case VAR_STAGENEWXBOUNDARY1: break;
                    case VAR_STAGENEWXBOUNDARY2: break;
                    case VAR_STAGENEWYBOUNDARY1: break;
                    case VAR_STAGENEWYBOUNDARY2: break;
                    case VAR_STAGECURXBOUNDARY1: break;
                    case VAR_STAGECURXBOUNDARY2: break;
                    case VAR_STAGECURYBOUNDARY1: break;
                    case VAR_STAGECURYBOUNDARY2: break;
                    case VAR_STAGEDEFORMATIONDATA0: break;
                    case VAR_STAGEDEFORMATIONDATA1: break;
                    case VAR_STAGEDEFORMATIONDATA2: break;
                    case VAR_STAGEDEFORMATIONDATA3: break;
                    case VAR_STAGEWATERLEVEL: break;
                    case VAR_STAGEACTIVELAYER: break;
                    case VAR_STAGEMIDPOINT: break;
                    case VAR_STAGEPLAYERLISTPOS: break;
                    case VAR_STAGEDEBUGMODE: break;
                    case VAR_STAGEENTITYPOS: break;
                    case VAR_SCREENCAMERAENABLED: break;
                    case VAR_SCREENCAMERATARGET: break;
                    case VAR_SCREENCAMERASTYLE: break;
                    case VAR_SCREENCAMERAX: break;
                    case VAR_SCREENCAMERAY: break;
                    case VAR_SCREENDRAWLISTSIZE: break;
                    case VAR_SCREENXCENTER: break;
                    case VAR_SCREENYCENTER: break;
                    case VAR_SCREENXSIZE: break;
                    case VAR_SCREENYSIZE: break;
                    case VAR_SCREENXOFFSET: break;
                    case VAR_SCREENYOFFSET: break;
                    case VAR_SCREENSHAKEX: break;
                    case VAR_SCREENSHAKEY: break;
                    case VAR_SCREENADJUSTCAMERAY: break;
                    case VAR_TOUCHSCREENDOWN: break;
                    case VAR_TOUCHSCREENXPOS: break;
                    case VAR_TOUCHSCREENYPOS: break;
                    case VAR_MUSICVOLUME: break;
                    case VAR_MUSICCURRENTTRACK: break;
                    case VAR_MUSICPOSITION: break;
                    case VAR_KEYDOWNUP: break;
                    case VAR_KEYDOWNDOWN: break;
                    case VAR_KEYDOWNLEFT: break;
                    case VAR_KEYDOWNRIGHT: break;
                    case VAR_KEYDOWNBUTTONA: break;
                    case VAR_KEYDOWNBUTTONB: break;
                    case VAR_KEYDOWNBUTTONC: break;
                    case VAR_KEYDOWNBUTTONX: break;
                    case VAR_KEYDOWNBUTTONY: break;
                    case VAR_KEYDOWNBUTTONZ: break;
                    case VAR_KEYDOWNBUTTONL: break;
                    case VAR_KEYDOWNBUTTONR: break;
                    case VAR_KEYDOWNSTART: break;
                    case VAR_KEYDOWNSELECT: break;
                    case VAR_KEYPRESSUP: break;
                    case VAR_KEYPRESSDOWN: break;
                    case VAR_KEYPRESSLEFT: break;
                    case VAR_KEYPRESSRIGHT: break;
                    case VAR_KEYPRESSBUTTONA: break;
                    case VAR_KEYPRESSBUTTONB: break;
                    case VAR_KEYPRESSBUTTONC: break;
                    case VAR_KEYPRESSBUTTONX: break;
                    case VAR_KEYPRESSBUTTONY: break;
                    case VAR_KEYPRESSBUTTONZ: break;
                    case VAR_KEYPRESSBUTTONL: break;
                    case VAR_KEYPRESSBUTTONR: break;
                    case VAR_KEYPRESSSTART: break;
                    case VAR_KEYPRESSSELECT: break;
                    case VAR_MENU1SELECTION: break;
                    case VAR_MENU2SELECTION: break;
                    case VAR_TILELAYERXSIZE: break;
                    case VAR_TILELAYERYSIZE: break;
                    case VAR_TILELAYERTYPE: break;
                    case VAR_TILELAYERANGLE: {
                        break;
                    }
                    case VAR_TILELAYERXPOS: break;
                    case VAR_TILELAYERYPOS: break;
                    case VAR_TILELAYERZPOS: break;
                    case VAR_TILELAYERPARALLAXFACTOR: break;
                    case VAR_TILELAYERSCROLLSPEED: break;
                    case VAR_TILELAYERSCROLLPOS: break;
                    case VAR_TILELAYERDEFORMATIONOFFSET: break;
                    case VAR_TILELAYERDEFORMATIONOFFSETW: break;
                    case VAR_HPARALLAXPARALLAXFACTOR: break;
                    case VAR_HPARALLAXSCROLLSPEED: break;
                    case VAR_HPARALLAXSCROLLPOS: break;
                    case VAR_VPARALLAXPARALLAXFACTOR: break;
                    case VAR_VPARALLAXSCROLLSPEED: break;
                    case VAR_VPARALLAXSCROLLPOS: break;
                    case VAR_SCENE3DVERTEXCOUNT: break;
                    case VAR_SCENE3DFACECOUNT: break;
                    case VAR_SCENE3DPROJECTIONX: break;
                    case VAR_SCENE3DPROJECTIONY: break;
                    case VAR_SCENE3DFOGCOLOR: break;
                    case VAR_SCENE3DFOGSTRENGTH: break;
                    case VAR_VERTEXBUFFERX: break;
                    case VAR_VERTEXBUFFERY: break;
                    case VAR_VERTEXBUFFERZ: break;
                    case VAR_VERTEXBUFFERU: break;
                    case VAR_VERTEXBUFFERV: break;
                    case VAR_FACEBUFFERA: break;
                    case VAR_FACEBUFFERB: break;
                    case VAR_FACEBUFFERC: break;
                    case VAR_FACEBUFFERD: break;
                    case VAR_FACEBUFFERFLAG: break;
                    case VAR_FACEBUFFERCOLOR: break;
                    case VAR_SAVERAM: break;
                    case VAR_ENGINESTATE:; break;
                    case VAR_ENGINELANGUAGE: break;
                    case VAR_ENGINEONLINEACTIVE: break;
                    case VAR_ENGINESFXVOLUME: break;
                    case VAR_ENGINEBGMVOLUME: break;
                    case VAR_ENGINETRIALMODE:; break;
                    case VAR_ENGINEDEVICETYPE:
                        break;
                        // EDITOR-ONLY
                    case VAR_EDITORVARIABLEID: break;
                    case VAR_EDITORVARIABLEVAL:
                        scnEditor->viewer->variableValue = scriptEng.operands[i];
                        break;
                    case VAR_EDITORRETURNVAR: break;
                    case VAR_EDITORSHOWGIZMOS: break;
                    case VAR_EDITORDRAWINGOVERLAY:
                        scnEditor->viewer->sceneInfo.debugMode = scriptEng.operands[i];
                        break;
                }
            }
            else if (opcodeType == SCRIPTVAR_INTCONST) { // int constant
                scriptCodePtr++;
            }
            else if (opcodeType == SCRIPTVAR_STRCONST) { // string constant
                int strLen = scriptCode[scriptCodePtr++];
                for (int c = 0; c < strLen; ++c) {
                    switch (c % 4) {
                        case 0: break;
                        case 1: break;
                        case 2: break;
                        case 3: ++scriptCodePtr; break;
                        default: break;
                    }
                }
                scriptCodePtr++;
            }
        }
    }
}

byte Compilerv4::ArcTanLookup(int X, int Y)
{
    int XVal;
    byte result = 0;
    int YVal;

    if (X >= 0)
        XVal = X;
    else
        XVal = -X;

    if (Y >= 0)
        YVal = Y;
    else
        YVal = -Y;

    if (XVal <= YVal) {
        while (YVal > 0xFF) {
            XVal >>= 4;
            YVal >>= 4;
        }
    }
    else {
        while (XVal > 0xFF) {
            XVal >>= 4;
            YVal >>= 4;
        }
    }
    if (X <= 0) {
        if (Y <= 0)
            result = atanVal256[0x100 * XVal + YVal] + -0x80;
        else
            result = -0x80 - atanVal256[0x100 * XVal + YVal];
    }
    else if (Y <= 0)
        result = -atanVal256[0x100 * XVal + YVal];
    else
        result = atanVal256[0x100 * XVal + YVal];
    return result;
}
