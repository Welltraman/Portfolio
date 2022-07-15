using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CGeneral : CharacterBase
{
    protected override void GetStats() {
        damage = 1.0f;
        health = 1.5f;
        speed = 1.0f;
    }
    public override Effects ApplyEffect() {
        Effects e = new Effects();
        e.damage = 0.3f;
        e.direction = new float[] {-Mathf.Infinity, 0.0f};
        return e;
    }
    protected override IEnumerator Attack() { return null; }
    public override string GetName() { return "General"; }
    public override string GetDescription() { return "Leader."; }
}
