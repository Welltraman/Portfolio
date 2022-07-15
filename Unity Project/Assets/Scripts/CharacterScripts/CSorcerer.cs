using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CSorcerer : CharacterBase
{
    protected override void GetStats() {
        damage = 1.0f;
        health = 0.5f;
        speed = 1.0f;
    }
    public override Effects ApplyEffect() {
        Effects e = new Effects();
        float randBuffMax = 0.5f;
        int randIdx = Random.Range(0, 3);
        switch (randIdx) {
            case 0:
                e.damage = Random.Range(0, randBuffMax);
                break;
            case 1:
                e.health = Random.Range(0, randBuffMax);
                break;
            case 2:
                e.speed = Random.Range(0, randBuffMax);
                break;
        }
        e.direction = new float[] { Random.Range(0, transform.parent.childCount) };
        return e;
    }
    protected override IEnumerator Attack() { return null; }
    public override string GetName() { return "Sorcerer"; }
    public override string GetDescription() { return "Shoots magic."; }
}
