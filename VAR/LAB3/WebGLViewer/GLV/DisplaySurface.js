// Class DisplaySurface: Each of the walls of a CAVE.
DisplaySurface = function(orig, uvector, vvector){ // (orig = Vec3, uvector = Vec3, vvector = Vec3)

  this.origin = orig; // Vec3 - Origin of the display
  this.u = uvector;   // Vec3 - Horizontal vector
  this.v = vvector;   // Vec3 - Vertical vector

};

// Functions

DisplaySurface.prototype.viewingMatrix = function(eye) {
  var mat = new Mat4();
  mat.loadIdentity();

  var newU = new Vec3(this.u.x, this.u.y, this.u.z);
  newU.normalize();
  var newV = new Vec3(this.v.x, this.v.y, this.v.z);
  newV.normalize();
  var normal = Vec3.cross(newU, newV);

  mat._[0] = newU.x;
  mat._[1] = newU.y;
  mat._[2] = newU.z;
  mat._[3] = - Vec3.dot(newU, eye);
  mat._[4] = newV.x;
  mat._[5] = newV.y;
  mat._[6] = newV.z;
  mat._[7] = - Vec3.dot(newV, eye);
  mat._[8] = normal.x;
  mat._[9] = normal.y;
  mat._[10] = normal.z;
  mat._[11] = - Vec3.dot(normal, eye);
  mat._[12] = 0;
  mat._[13] = 0;
  mat._[14] = 0;
  mat._[15] = 1;

  mat.transpose();

  return mat;
};

DisplaySurface.prototype.projectionMatrix = function(eye, znear, zfar){
  var mat = new Mat4();
  mat.loadIdentity();

  var newU = new Vec3(this.u.x, this.u.y, this.u.z);
  newU.normalize();
  var newV = new Vec3(this.v.x, this.v.y, this.v.z);
  newV.normalize();
  var normal = Vec3.cross(newU, newV);

  var l,r,b,t,n,f;
  n=znear;
  f=zfar;
  l=-n*(150+Vec3.dot(newU, eye))/(150+Vec3.dot(normal, eye));
  r=n*(150-Vec3.dot(newU, eye))/(150+Vec3.dot(normal, eye));
  t=-n*(150+Vec3.dot(newV, eye))/(150+Vec3.dot(normal, eye));
  b=n*(150-Vec3.dot(newV, eye))/(150+Vec3.dot(normal, eye));

  mat.frustum(l,r,t,b,n,f);

  return mat;



};
