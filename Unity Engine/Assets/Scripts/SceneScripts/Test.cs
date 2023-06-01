using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class Test : MonoBehaviour, IDropHandler, IPointerDownHandler
{
    public void OnDrop(PointerEventData pData) {
        print("YEP");
    }
    public void OnPointerDown(PointerEventData pData) {
        print("DOWN");
    }
}
