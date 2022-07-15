using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class CharacterDescription : MonoBehaviour
{
    public void AddDescription(GameObject pCharacter) {
        CharacterBase character = pCharacter.GetComponent<CharacterBase>();
        transform.GetChild(0).GetComponent<TextMeshProUGUI>().text = character.GetName();
        transform.GetChild(1).GetComponent<TextMeshProUGUI>().text = character.GetDescription();
    }
}
