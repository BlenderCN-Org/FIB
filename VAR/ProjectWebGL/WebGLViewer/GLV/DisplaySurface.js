// Class DisplaySurface: Each of the walls of a CAVE.
DisplaySurface = function(orig, uvector, vvector){ // (orig = Vec3, uvector = Vec3, vvector = Vec3)
    
    this.origin = orig; // Vec3 - Origin of the display
    this.u = uvector;   // Vec3 - Horizontal vector
    this.v = vvector;   // Vec3 - Vertical vector

};

// Functions

DisplaySurface.prototype.viewingMatrix = function(eye){
	var mat = new Mat4();
    mat.loadIdentity();

    this.u.normalize();
    this.v.normalize();


	return mat;
};

DisplaySurface.prototype.projectionMatrix = function(eye, znear, zfar){
    var mat = new Mat4();
	mat.loadIdentity();

	this.u.normalize();
    this.v.normalize();

    var normal = u.cross(v);

    var l,r,b,t,n,f;
    n=znear;
    f=zfar;
    l=-n*(150+u.dot(eye))/(150+normal.dot(eye));
    r=n*(150-u.dot(eye))/(150+normal.dot(eye));
    t=-n*(150+v.dot(eye))/(150+normal.dot(eye));
    b=n*(150-v.dot(eye))/(150+normal.dot(eye));

    mat.frustrum(l,r,t,b,n,f);

	return mat;
};
