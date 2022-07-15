#pragma once
#include "editor/editor_scale.h"
#include "editor/plugins/resource_preloader_editor_plugin.h"
#include "print_string_extended.hpp"

const String RESOURCE_MANAGER_SETTING = "resource_manager/loaded_assets";
const String RESOURCE_MANAGER_DIRECTORY_SETTING = "resource_manager/directory";

class ResourceManager : public ResourcePreloader {
	GDCLASS(ResourceManager, ResourcePreloader);

	class FileLoader {
		Variant _load_resource(const String& a_file_path) const;
		Array _load_resources(DirAccess* p_dir) const;
		DirAccess* _setup_dir_access(const String& a_directory_path) const;
	public:
		Variant load(const String& a_file_path) const;
	};

	typedef Variant(ResourceManager::*fptr)(const String&);
	Vector<fptr> m_getters;
	Variant _get_from_file_loader(const String& a_text);
	Variant _get_from_group(const String& a_text);
	Variant _get_from_preloader(const String& a_text);
	Variant _get_from_scene(const String& a_text);
	String _get_full_path(String a_text) const;
	Vector<fptr> _setup_getters();
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("find", "name"), &ResourceManager::get_asset);
		ClassDB::bind_method(D_METHOD("remove", "name"), &ResourceManager::remove);
		ClassDB::bind_method(D_METHOD("save"), &ResourceManager::save);
		ClassDB::bind_method(D_METHOD("get_dir", "extension"), &ResourceManager::get_directory);
		ClassDB::bind_method(D_METHOD("initialize"), &ResourceManager::initialize);
	}
public:
	static ResourceManager* singleton;
	Variant get_asset(const String& a_text);
	String get_directory(const String &a_text);
	void initialize();
	void remove(const String& a_name);
	void save();
	ResourceManager();
	~ResourceManager() { singleton = nullptr; }
};

class ResourceManagerEditor : public PanelContainer {
	GDCLASS(ResourceManagerEditor, PanelContainer);
	enum {
		BUTTON_OPEN_SCENE,
		BUTTON_EDIT_RESOURCE,
		BUTTON_REMOVE
	};
	ResourceManager* m_placeholder;
	Tree* m_tree;
	UndoRedo* m_undo_redo;
	void _add_resource(const String& a_name, const RES& a_res);
	bool _can_drop_data_fw(const Point2& a_point, const Variant& a_data, Control* p_from) const;
	void _cell_button_pressed(Object* p_item, int a_column, int a_id);
	void _drop_data_fw(const Point2& a_point, const Variant& a_data, Control* p_from);
	void _edit_resource(const String& a_old, const String& a_new, const RES& a_res);
	Variant _get_drag_data_fw(const Point2& a_point, Control* p_from);
	String _get_resource_name(const RES& a_res);
	void _files_load_request(const Vector<String>& a_paths);
	void _initialize_tree();
	void _item_edited();
	void _remove_resource(const String& a_name, const RES& a_res);
	void _update_library();
	void _update_tree(List<String>& r_names);
	void _update_tree_create_label(TreeItem* p_ti, const String& a_name);
	void _update_tree_create_buttons(TreeItem* p_ti, const String& a_type, const RES& a_res);
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("_cell_button_pressed"), &ResourceManagerEditor::_cell_button_pressed);
		ClassDB::bind_method(D_METHOD("_item_edited"), &ResourceManagerEditor::_item_edited);
		ClassDB::bind_method(D_METHOD("_update_library"), &ResourceManagerEditor::_update_library);
		ClassDB::bind_method(D_METHOD("can_drop_data_fw"), &ResourceManagerEditor::_can_drop_data_fw);
		ClassDB::bind_method(D_METHOD("drop_data_fw"), &ResourceManagerEditor::_drop_data_fw);
		ClassDB::bind_method(D_METHOD("get_drag_data_fw"), &ResourceManagerEditor::_get_drag_data_fw);
	}
public:
	void edit();
	void set_undo_redo(UndoRedo *p_undo_redo) { m_undo_redo = p_undo_redo; }
	ResourceManagerEditor();
};

class ResourceManagerPlugin : public EditorPlugin {
	GDCLASS(ResourceManagerPlugin, EditorPlugin);
	ResourceManagerEditor* m_manager_editor;
	void _on_enter_tree();
protected:
	void _notification(int a_what);
public:
	ResourceManagerPlugin(EditorNode* p_node);
};
