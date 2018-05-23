// Class Object: instance of a polygonal mesh.
GLV.Object = function(meshName){
    this.pos = new Vec3(0, 0, 0);
    this.phi = 0; // Vertical angle
    this.theta = 0; // Horizontal angle
    this.scale = new Vec3(1.0, 1.0, 1.0);
    this.mesh = meshName; // Name of the corresponding mesh
};

GLV.Object.prototype.draw = function(mvmat, pmat){
	mvm = mvmat.copy();
	pm = pmat.copy();
	
    var shaderProg = GLV.ShaderManager.getActiveShader();
    
    mvm.translate(this.pos);
    mvm.rotate(GLV.Utils.degToRad(this.phi), GLV.X_AXIS);
    mvm.rotate(GLV.Utils.degToRad(this.theta), GLV.Y_AXIS);
    mvm.scale(this.scale.toArray());
    
    var nm = Mat4.identity();
    nm.set(mvm);
    nm.invert();
    nm.transpose();
    
    // Set matrices
    gl.uniformMatrix4fv(shaderProg.uPMatrix, false, pm.toArray());
    gl.uniformMatrix4fv(shaderProg.uMVMatrix, false, mvm.toArray());
    gl.uniformMatrix4fv(shaderProg.uNMatrix, false, nm.toArray());
    
    // Draw the mesh
    GLV.scene.meshes[this.mesh].draw();
};
