using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CHealer : CharacterBase
{
    protected override void GetStats() {
        damage = 0.2f;
        health = 1.5f;
        speed = 1.0f;
    }
    public override Effects ApplyEffect() { return null; }
    protected override IEnumerator Attack() { return null; }
    public override string GetName() { return "Healer"; }
    public override string GetDescription() { return "Cute girl."; }
}
