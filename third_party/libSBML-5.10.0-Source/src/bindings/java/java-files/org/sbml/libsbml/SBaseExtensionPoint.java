/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.sbml.libsbml;

/** 
 *  Representation of an extension point of SBML's package
 * extension.
 <p>
 * @internal 
 <p>
 * {@link SBaseExtensionPoint} represents an element to be extended (extension point) and the
 * extension point is identified by a combination of a package name and a typecode of the 
 * element.
 <p>
 <p>
 * For example, an {@link SBaseExtensionPoint} object which represents an extension point of the model
 * element defined in the <em>core</em> package can be created as follows:
 <p>
<div class='fragment'><pre class='fragment'>
      {@link SBaseExtensionPoint}  modelextp('core', SBML_MODEL);
</pre></div>
 <p>
 * Similarly, an {@link SBaseExtensionPoint} object which represents an extension point of
 * the layout element defined in the layout extension can be created as follows:
 <p>
<div class='fragment'><pre class='fragment'>
      {@link SBaseExtensionPoint}  layoutextp('layout', SBML_LAYOUT_LAYOUT);
</pre></div>
 <p>
 * {@link SBaseExtensionPoint} object is required as one of arguments of the constructor 
 * of SBasePluginCreator&lt;class SBasePluginType, class SBMLExtensionType&gt;
 * template class to identify an extension poitnt to which the plugin object created
 * by the creator class is plugged in.
 * For example, the SBasePluginCreator class which creates a LayoutModelPlugin object
 * of the layout extension which is plugged in to the model element of the <em>core</em>
 * package can be created with the corresponding {@link SBaseExtensionPoint} object as follows:
 <p>
<div class='fragment'><pre class='fragment'>
  // std.vector object that contains a list of URI (package versions) supported 
  // by the plugin object.
  std.vector&lt;String&gt; packageURIs;
  packageURIs.push_back(getXmlnsL3V1V1());
  packageURIs.push_back(getXmlnsL2());  

  // creates an extension point (model element of the 'core' package)
  {@link SBaseExtensionPoint}  modelExtPoint('core',SBML_MODEL);
   
  // creates an SBasePluginCreator object 
  SBasePluginCreator&lt;LayoutModelPlugin, LayoutExtension&gt;  modelPluginCreator(modelExtPoint,packageURIs);
</pre></div>
 <p>
 * This kind of code is implemented in init() function of each {@link SBMLExtension} derived classes.
 */

public class SBaseExtensionPoint {
   private long swigCPtr;
   protected boolean swigCMemOwn;

   protected SBaseExtensionPoint(long cPtr, boolean cMemoryOwn)
   {
     swigCMemOwn = cMemoryOwn;
     swigCPtr    = cPtr;
   }

   protected static long getCPtr(SBaseExtensionPoint obj)
   {
     return (obj == null) ? 0 : obj.swigCPtr;
   }

   protected static long getCPtrAndDisown (SBaseExtensionPoint obj)
   {
     long ptr = 0;

     if (obj != null)
     {
       ptr             = obj.swigCPtr;
       obj.swigCMemOwn = false;
     }

     return ptr;
   }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        libsbmlJNI.delete_SBaseExtensionPoint(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  
/**
   * constructor
   */ public
 SBaseExtensionPoint(String pkgName, int typeCode) {
    this(libsbmlJNI.new_SBaseExtensionPoint__SWIG_0(pkgName, typeCode), true);
  }

  
/**
   * copy constructor
   */ public
 SBaseExtensionPoint(SBaseExtensionPoint rhs) {
    this(libsbmlJNI.new_SBaseExtensionPoint__SWIG_1(SBaseExtensionPoint.getCPtr(rhs), rhs), true);
  }

  
/**
   * Creates and returns a deep copy of this {@link SBaseExtensionPoint}.
   <p>
   * @return a (deep) copy of this {@link SBaseExtensionPoint}.
   */ public
 SBaseExtensionPoint cloneObject() {
    long cPtr = libsbmlJNI.SBaseExtensionPoint_cloneObject(swigCPtr, this);
    return (cPtr == 0) ? null : new SBaseExtensionPoint(cPtr, true);
  }

  
/**
   * Returns the package name of this extension point.
   */ public
 String getPackageName() {
    return libsbmlJNI.SBaseExtensionPoint_getPackageName(swigCPtr, this);
  }

  
/**
   * Returns the typecode of this extension point.
   */ public
 int getTypeCode() {
    return libsbmlJNI.SBaseExtensionPoint_getTypeCode(swigCPtr, this);
  }

}
