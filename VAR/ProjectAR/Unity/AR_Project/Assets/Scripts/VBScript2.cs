using System.Collections;
using UnityEngine;
using Vuforia;

public class VBScript2 : MonoBehaviour, IVirtualButtonEventHandler {

	private GameObject VBButtonObject;
	private GameObject Tigre;

	// Use this for initialization
	void Start () {

		VBButtonObject = GameObject.Find ("rotatebutton");
		VBButtonObject.GetComponent<VirtualButtonBehaviour> ().RegisterEventHandler (this);
		Tigre = GameObject.Find ("Tigre");
	}


	public void OnButtonPressed (VirtualButtonAbstractBehaviour vb){

		Debug.Log ("pressed");
		Tigre.transform.Rotate(new Vector3 (0, Time.deltaTime * 2000, 0));

	}


	public void OnButtonReleased( VirtualButtonAbstractBehaviour vb ) { 

		Debug.Log ("unpressed"); 

	}

}