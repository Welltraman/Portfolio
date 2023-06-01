using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AreaTrigger : MonoBehaviour {
    private EffectTarget mTarget;
    public void Initialize(EffectTarget pTarget) => mTarget = pTarget;
    void OnTriggerEnter2D(Collider2D col) => mTarget.OnTriggerEnter(col);
    void OnTriggerExit2D(Collider2D col) => mTarget.OnTriggerExit(col);
}
