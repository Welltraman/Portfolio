using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterFactory : MonoBehaviour {
    public List<GameObject> characters = new List<GameObject>();
    public List<GameObject> GetRandomCharacters(int pNum) {
        if (characters.Count == 0 || pNum == 0) return null;
        List<GameObject> l = new List<GameObject>();
        for (int i = 0; i < pNum; i++) {
            l.Add(characters[Random.Range(0, characters.Count)]);
        }
        return l;
    }
    public GameObject GetCharacter(string pName) {
        foreach (GameObject obj in characters) {
            if (obj.name == pName) return obj;
        }
        return null;
    }
}
