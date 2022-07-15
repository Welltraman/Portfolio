using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CKnight : CharacterBase
{
    protected override void GetStats() {
        damage = 1.0f;
        health = 1.0f;
        speed = 1.0f;
    }
    public override Effects ApplyEffect() {
        Effects e = new Effects();
        e.health = 0.5f;
        e.direction = new float[] {-Mathf.Infinity, 0.0f};
        return e;
    }
    protected override IEnumerator Attack() { return null; }
    public override string GetName() { return "Knight"; }
    public override string GetDescription() { return "Wears heavy armour."; }
}
