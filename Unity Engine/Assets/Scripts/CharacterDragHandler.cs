using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class CharacterDragHandler : MonoBehaviour, IPointerDownHandler, IPointerUpHandler, IDragHandler {
	[HideInInspector] public GameObject disabledImage;
	private CanvasGroup mCanvasGroup;
	void Awake() {
		mCanvasGroup = GetComponent<CanvasGroup>();
	}
	public void OnPointerDown(PointerEventData pData) {
		if (disabledImage == null) {
			disabledImage = GameManager.GetSceneComponent<CharacterPicker>().AddDisabledImage(gameObject);
		}
		transform.parent.GetComponent<CharacterPicker>().runButton.GetComponent<Image>().raycastTarget = false;
		GameManager.GetSceneComponent<CharacterDescription>().AddDescription(gameObject);
		mCanvasGroup.blocksRaycasts = false;
		mCanvasGroup.alpha = 0.6f;
	}
	public void OnPointerUp(PointerEventData pData) {
		mCanvasGroup.alpha = 1;
		GameObject hit = pData.pointerCurrentRaycast.gameObject;
		CharacterPicker picker = transform.parent.GetComponent<CharacterPicker>();
		if (hit && hit.transform.IsChildOf(transform.parent)) picker.BringBackImage(gameObject);
		picker.runButton.GetComponent<Image>().raycastTarget = true;
	}
	public void OnDrag(PointerEventData pData) {
		GetComponent<RectTransform>().anchoredPosition += pData.delta;
	}
}
