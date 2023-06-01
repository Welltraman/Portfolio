using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class CharacterPicker : MonoBehaviourPlus {
	public GameObject characterDrag, characterDisabled;
	public Button runButton;
	protected override void OnGameState(GameStateEnum pState) {
		switch (pState) {
			case GameStateEnum.SIMULATE:
				foreach (Transform child in transform) child.GetComponent<Image>().raycastTarget = false;
				runButton.interactable = false;
				break;
			case GameStateEnum.ROUND_BEGIN:
				foreach (Transform child in transform) child.GetComponent<Image>().raycastTarget = true;
				runButton.interactable = true;
				break;
		}
    }
	public void Initialize(List<GameObject> pCharacters) {
		if (pCharacters != null) {
			Vector2 imgSize = characterDrag.GetComponent<RectTransform>().sizeDelta;
			GetComponent<RectTransform>().sizeDelta = new Vector2(pCharacters.Count * imgSize.x, imgSize.y);
			for (int i = 0; i < pCharacters.Count; i++) AddImageAtPicker(pCharacters[i]);
		}
	}
	private void AddImageAtPicker(GameObject pCharacterRaw) {
		GameObject obj = Instantiate(characterDrag, this.transform);
		obj.name = pCharacterRaw.name;
		obj.GetComponent<Image>().sprite = pCharacterRaw.GetComponent<SpriteRenderer>().sprite;
		RectTransform rect = obj.GetComponent<RectTransform>();
		int handSize = Settings.instance.startingHand;
		float anchorX = 1f / (handSize == 1 ? 1 : handSize - 1) * obj.transform.GetSiblingIndex();
		rect.anchorMin = new Vector2(anchorX, 0.5f);
		rect.anchorMax = new Vector2(anchorX, 0.5f);
		rect.pivot = new Vector2(anchorX, 0.5f);
	}
	public GameObject AddDisabledImage(GameObject pCharacterImage) {
		GameObject obj = Instantiate(characterDisabled, this.transform);
		obj.name = pCharacterImage.name + " - disabled";
		obj.GetComponent<Image>().sprite = pCharacterImage.GetComponent<Image>().sprite;
		RectTransform rt = obj.GetComponent<RectTransform>();
		RectTransform rt_base = pCharacterImage.GetComponent<RectTransform>();
		rt.anchorMin = rt_base.anchorMin;
		rt.anchorMax = rt_base.anchorMax;
		rt.pivot = rt_base.pivot;
		return obj;
	}
	// Repositioning after/when dragged ===========================================================
	public void BringBackImage(GameObject pCharacterImage, bool pDestroyDisabled = true) {
		if (pDestroyDisabled)
			Destroy(pCharacterImage.GetComponent<CharacterDragHandler>().disabledImage);
		RectTransform rect = pCharacterImage.GetComponent<RectTransform>();
		rect.anchoredPosition = Vector3.zero;
		rect.pivot = rect.anchorMax;
		pCharacterImage.GetComponent<CanvasGroup>().blocksRaycasts = true;
	}
	public void EnableCharacterDrag(GameObject pDrag, PointerEventData pData) {
		Vector2 pos;
		Canvas canvas = transform.parent.GetComponent<Canvas>();
		RectTransform rect = pDrag.GetComponent<RectTransform>();
        RectTransformUtility.ScreenPointToLocalPointInRectangle(rect, Input.mousePosition, canvas.worldCamera, out pos);
		rect.anchoredPosition = pos;
		rect.pivot = new Vector2(0.45f, 0.5f);
		ExecuteEvents.Execute<IPointerDownHandler>(pDrag, pData, ExecuteEvents.pointerDownHandler);
	}
}