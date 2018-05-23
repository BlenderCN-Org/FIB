// Class CAVE: Simulates a Cave Automatic Virtual Environment.
// The used metric is meters. 1 unit in world space = 1 centimeter.
var CAVE = {

    // Variables

    leftEye : new Vec3(-3.0, 20.0, 50.0),
    rightEye : new Vec3(3.0, 20.0, 50.0),
    near : 1.0,
    far : 10000.0,
    TEX_SIZE : 1024,

    front : new DisplaySurface(new Vec3(-150.0, -150.0, -150.0), new Vec3(300.0, 0.0,   0.0), new Vec3(0.0, 300.0,   0.0)),
    left : new DisplaySurface(new Vec3(-150.0, -150.0, 150.0), new Vec3(0.0, 0.0,  -300.0), new Vec3(0.0, 300.0,   0.0)),
    right : new DisplaySurface(new Vec3(150.0, -150.0, -150.0), new Vec3(0.0, 0.0, 300.0), new Vec3(0.0, 300.0,   0.0)),
    floor : new DisplaySurface(new Vec3(-150.0, -150.0, 150.0), new Vec3(300.0, 0.0,   0.0), new Vec3(0.0, 0.0,  -300.0)),



    // Functions

    init : function(){
        // Init the display surface 3D objects
        var objs = [
            // [MODEL_NAME, POSITION = [X, Y, Z], SCALE = [X, Y, Z], ROTATION = [theta_deg, phi_deg]]
            [ "EyeL", [this.leftEye.x, this.leftEye.y, this.leftEye.z], [-2.0, 2.0, 2.0] ], // Left eye
            [ "EyeR", [this.rightEye.x, this.rightEye.y, this.rightEye.z], [2.0, 2.0, 2.0] ], // Right eye﻿
            [ "Plane", [   0.0,    0.0, -150.0], [300.0, 300.0, 1.0] ], // Front plane
            [ "Plane", [   -150.0,    0.0, 0.0], [300.0, 300.0, 1.0], [90.0,0.0] ], // Left plane
            [ "Plane", [   150.0,    0.0, 0.0], [300.0, 300.0, 1.0], [-90.0,0.0] ], // Right plane
            [ "Plane", [   0.0,    -150.0, 0.0], [300.0, 300.0, 1.0], [0.0,-90.0] ], // Floor plane
        ];
        this.objects = [];
        for (var i = 0; i < objs.length; ++i){
            var objData = objs[i];
            this.objects[i] = new GLV.Object(objData[0]);
            if (objData[1] instanceof Array) this.objects[i].pos = new Vec3(objData[1]);
            if (objData[2] instanceof Array) this.objects[i].scale = new Vec3(objData[2]);
            if (objData[3] instanceof Array){
                this.objects[i].theta = objData[3][0];
                this.objects[i].phi = objData[3][1];
            }
        }

        // Init textures for each display surface
        GLV.TextureManager.createTexture("front", this.TEX_SIZE, this.TEX_SIZE, true);
        GLV.TextureManager.createTexture("left", this.TEX_SIZE, this.TEX_SIZE, true);
        GLV.TextureManager.createTexture("right", this.TEX_SIZE, this.TEX_SIZE, true);
        GLV.TextureManager.createTexture("floor", this.TEX_SIZE, this.TEX_SIZE, true);


        // Init keyboard callbacks
        var checkMoveScene = function(){ return GLV.uiSelectedMoveMode === GLV.MOVEMODE_SCENE; };
        var checkMoveEyes = function(){ return GLV.uiSelectedMoveMode === GLV.MOVEMODE_EYES; };
        var callbacks = [
            // Each element: callback = [ KEY | BOOL_FUNC | [KEY|BOOL_FUNC], FUNCTION() | [elem]]
            [ checkMoveScene, [ [GLV.Input.D, function(){ GLV.scene.pos.add(new Vec3( 5.0,  0.0,  0.0)); } ],
                                 [GLV.Input.A, function(){ GLV.scene.pos.add(new Vec3(-5.0,  0.0,  0.0)); }],
                                 [GLV.Input.E, function(){ GLV.scene.pos.add(new Vec3( 0.0,  5.0,  0.0)); } ],
                                 [GLV.Input.Q, function(){ GLV.scene.pos.add(new Vec3( 0.0, -5.0,  0.0)); } ],
                                 [GLV.Input.S, function(){ GLV.scene.pos.add(new Vec3( 0.0,  0.0,  5.0)); } ],
                                 [GLV.Input.W, function(){ GLV.scene.pos.add(new Vec3( 0.0,  0.0, -5.0)); } ] ]
            ],
            [ checkMoveEyes, [ [GLV.Input.D, function(){ CAVE.moveEyes(new Vec3( 2.5,  0.0,  0.0)); } ],
                               [GLV.Input.A, function(){ CAVE.moveEyes(new Vec3(-2.5,  0.0,  0.0)); }],
                               [GLV.Input.E, function(){ CAVE.moveEyes(new Vec3( 0.0,  2.5,  0.0)); } ],
                               [GLV.Input.Q, function(){ CAVE.moveEyes(new Vec3( 0.0, -2.5,  0.0)); } ],
                               [GLV.Input.S, function(){ CAVE.moveEyes(new Vec3( 0.0,  0.0,  2.5)); } ],
                               [GLV.Input.W, function(){ CAVE.moveEyes(new Vec3( 0.0,  0.0, -2.5)); } ] ]
            ]
        ];
        GLV.Input.addCallbacks(callbacks);

        // Scale scene callbacks
        GLV.Input.scrollPositiveCallbacks.push(
            function(){
                if (GLV.uiSelectedMoveMode === GLV.MOVEMODE_SCENE) GLV.scene.incrScale(new Vec3(0.1, 0.1, 0.1));
            }
        );
        GLV.Input.scrollNegativeCallbacks.push(
            function(){
                if (GLV.uiSelectedMoveMode === GLV.MOVEMODE_SCENE) GLV.scene.incrScale(new Vec3(-0.1, -0.1, -0.1));
            }
        );

        // Change interocular distance callbacks
        GLV.Input.scrollPositiveCallbacks.push(
            function(){
                if (GLV.uiSelectedMoveMode === GLV.MOVEMODE_EYES) CAVE.increaseEyeSeparation(6.0);
            }
        );
        GLV.Input.scrollNegativeCallbacks.push(
            function(){
                if (GLV.uiSelectedMoveMode === GLV.MOVEMODE_EYES) CAVE.increaseEyeSeparation(-6.0);
            }
        );
    },

    moveEyes : function(v){
        this.leftEye.add(v);
        this.rightEye.add(v);
        this.objects[0].pos.add(v);
        this.objects[1].pos.add(v);
    },

    increaseEyeSeparation : function(dist){
        if (this.leftEye.x - dist/2 > this.rightEye.x + dist/2) dist = this.leftEye.x - this.rightEye.x;
        this.leftEye.x -= dist/2;
        this.rightEye.x += dist/2;
        this.objects[0].pos.x -= dist/2;
        this.objects[1].pos.x += dist/2;
    },

	drawSceneOntoTexture : function(displaySurface, fboName){
    GLV.TextureManager.bindFBO(fboName);
    gl.colorMask(1,0,0,1);
		GLV.scene.draw(displaySurface.viewingMatrix(this.objects[0].pos), displaySurface.projectionMatrix(this.objects[0].pos,this.near,this.far));
    gl.colorMask(0,0,1,1);
    GLV.scene.draw(displaySurface.viewingMatrix(this.objects[1].pos), displaySurface.projectionMatrix(this.objects[1].pos,this.near,this.far));
		gl.bindFramebuffer(gl.FRAMEBUFFER, null);
    gl.colorMask(1,1,1,1);
	},

    draw : function(){
        // Adjust the viewport to the textures
        gl.viewport(0, 0, this.TEX_SIZE, this.TEX_SIZE);

        // For each DisplaySurface, draw scene onto a texture
        this.drawSceneOntoTexture(this.front, "front");
        this.drawSceneOntoTexture(this.left, "left");
        this.drawSceneOntoTexture(this.right, "right");
        this.drawSceneOntoTexture(this.floor, "floor");

        // Restore the gl context
        gl.viewport(0, 0, GLV.canvas.width, GLV.canvas.height);

        // Draw the regular scene
        GLV.scene.draw(GLV.camera.mvMat.copy(), GLV.camera.pMat.copy());

        // Draw the eyes of the user
        GLV.ShaderManager.setActiveShader(GLV.ShaderManager.PHONG);
        this.objects[0].draw(GLV.camera.mvMat, GLV.camera.pMat);
        this.objects[1].draw(GLV.camera.mvMat, GLV.camera.pMat);

        // Draw the texture-mapped DisplaySurfaces
        GLV.ShaderManager.setActiveShader(GLV.ShaderManager.CAVE_DISPLAY_SURFACE);
        var shaderProg = GLV.ShaderManager.getActiveShader();
        // Set lighting uniforms
        GLV.scene.light.setUniforms(shaderProg, GLV.camera.mvMat.copy());
        // Set DisplaySurfaces textures
        GLV.TextureManager.setUniformTexArray(["front", "left", "right", "floor"], shaderProg, "textures");

        gl.uniform1i(gl.getUniformLocation(shaderProg, "texIdx"), 0);
        this.objects[2].draw(GLV.camera.mvMat, GLV.camera.pMat);
        gl.uniform1i(gl.getUniformLocation(shaderProg, "texIdx"), 1);
        this.objects[3].draw(GLV.camera.mvMat, GLV.camera.pMat);
        gl.uniform1i(gl.getUniformLocation(shaderProg, "texIdx"), 2);
        this.objects[4].draw(GLV.camera.mvMat, GLV.camera.pMat);
        gl.uniform1i(gl.getUniformLocation(shaderProg, "texIdx"), 3);
        this.objects[5].draw(GLV.camera.mvMat, GLV.camera.pMat);


    }
};
