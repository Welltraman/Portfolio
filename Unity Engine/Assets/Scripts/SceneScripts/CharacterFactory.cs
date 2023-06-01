using System.Collections.Generic;
using UnityEngine;

public class CharacterFactory : MonoBehaviour {
    private static List<GameObject> mCharacters = new List<GameObject>();
    void Awake() {
        GameObject[] characters = Resources.LoadAll<GameObject>("Characters");
        foreach (GameObject character in characters) if (character.activeSelf) mCharacters.Add(character);
    }
    public static List<GameObject> GetCharacters(int pNum) {
        if (mCharacters.Count == 0 || pNum == 0) return null;
        List<GameObject> list = new List<GameObject>();
        for (int i = 0; i < pNum; i++) list.Add(mCharacters[Random.Range(0, mCharacters.Count)]);
        return list;
    }
    public static GameObject GetCharacter(string pName) {
        foreach (GameObject character in mCharacters) if (character.name == pName) return character;
        return null;
    }
}
