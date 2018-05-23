// Class ShaderManager: manager for the textures and FBOs of the program.
GLV.TextureManager = {
    
    // Dictionaries for storing textures and frame buffer objects
    textures : {},
    fbos : {},
    
    // Private methods
    
    setBasicTexParams : function(){
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    },
    
    // Public methods
    
    createTexture : function(name, width, height, needsFBO){
        if (this.textures[name] != undefined){
            GLV.err("The texture "+name+" already exists.");
            return;
        }
        var tex = gl.createTexture();
        tex.width = width;
        tex.height = height;
        gl.bindTexture(gl.TEXTURE_2D, tex);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, width, height, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);
        this.setBasicTexParams();
        gl.generateMipmap(gl.TEXTURE_2D);
        this.textures[name] = tex;
        
        if (needsFBO){
            // Create depth buffer
            var auxDepth = gl.createTexture();
            gl.bindTexture(gl.TEXTURE_2D, auxDepth);
            this.setBasicTexParams();
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.DEPTH_COMPONENT, width, height, 0, gl.DEPTH_COMPONENT, gl.UNSIGNED_SHORT, null);
            this.textures[name + "Depth"] = auxDepth;
            
            // Create the FBO
            var buf = gl.createFramebuffer();
            gl.bindFramebuffer(gl.FRAMEBUFFER, buf);
            gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, tex, 0);
            gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, gl.TEXTURE_2D, auxDepth, 0);
            this.fbos[name] = buf;
            gl.bindFramebuffer(gl.FRAMEBUFFER, null);
        }
    },
    
    getTexture : function(name){
        return this.textures[name];
    },
    
    bindFBO : function(name){
        var fbo = this.fbos[name];
        if (fbo != undefined) gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);
    },
    
    setTexUniform : function(texName, texUnit, shaderProg, uniformName){
        var tex = this.textures[texName];
        if (tex === undefined) return;
        gl.activeTexture(gl.TEXTURE0 + texUnit);
        gl.bindTexture(gl.TEXTURE_2D, tex);
        gl.uniform1i(gl.getUniformLocation(shaderProg, uniformName), texUnit);
    },
    
    setUniformTexArray : function(texNames, shaderProg, uniformName){
        var texUnits = [];
        for (var i = 0; i < texNames.length; ++i){
            gl.activeTexture(gl.TEXTURE0 + i);
            gl.bindTexture(gl.TEXTURE_2D, this.textures[ texNames[i] ]);
            texUnits[i] = i;
        }
        gl.uniform1iv(gl.getUniformLocation(shaderProg, uniformName), texUnits);
    }
};

