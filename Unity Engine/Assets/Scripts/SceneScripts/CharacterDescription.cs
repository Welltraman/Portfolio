using UnityEngine;
using TMPro;

public class CharacterDescription : MonoBehaviourPlus {
    public TMP_Text nameText;
    public TMP_Text roleText;
    public void AddDescription(GameObject pCharacter) {
        CharacterBase character = CharacterFactory.GetCharacter(pCharacter.name).GetComponent<CharacterBase>();
        nameText.text = character.GetName();
        roleText.text = character.GetDescription();
    }
}
