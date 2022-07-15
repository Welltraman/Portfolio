using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CThief : CharacterBase
{
    protected override void GetStats() {
        damage = 0.3f;
        health = 1.0f;
        speed = 1.5f;
    }
    public override Effects ApplyEffect() {
        Effects e = new Effects();
        e.speed = 0.2f;
        e.direction = new float[] {-Mathf.Infinity, 0.0f};
        return e;
    }
    protected override IEnumerator Attack() { return null; }
    public override string GetName() { return "Thief"; }
    public override string GetDescription() { return "Steals things."; }
}
