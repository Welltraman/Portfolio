using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class HealthBar : MonoBehaviour {
    public Slider slider;
    public TMP_Text text;
    void Awake() {
        CharacterBase character = transform.parent.GetComponent<CharacterBase>();
        slider.maxValue = character.health;
        slider.value = character.health;
        text.text = character.health.ToString();
    }
    public void OnHealthChange(float pHealth) {
        slider.value = pHealth;
        text.text = pHealth.ToString();
    }
}
