#include "resource_manager.hpp"

Variant ResourceManager::FileLoader::load(const String &a_file_path) const {
	if (a_file_path.get_extension() != "")
		return _load_resource(a_file_path);
	else {
		DirAccess *dir = _setup_dir_access(a_file_path);
		if (!dir)
			return Array();
		return _load_resources(dir);
	}
}

Variant ResourceManager::FileLoader::_load_resource(const String& a_file_path) const {
	if (FileAccess::exists(a_file_path) && ResourceLoader::exists(a_file_path))
		return ResourceLoader::load(a_file_path);
	print_raw(vformat("'%s' was not loaded.", a_file_path));
	return NULL;
}

Array ResourceManager::FileLoader::_load_resources(DirAccess* p_dir) const {
	Array array;
	String file = p_dir->get_next();
	while (file != "") {
		if (file != "." && file != "..")
			array.push_back(_load_resource(vformat("%s/%s", p_dir->get_current_dir(), file)));
		file = p_dir->get_next();
	}
	memdelete(p_dir);
	return array;
}

DirAccess* ResourceManager::FileLoader::_setup_dir_access(const String& a_directory_path) const {
	Error err;
	DirAccess *dir = DirAccess::open(a_directory_path, &err);
	if (err)
		return nullptr;
	ERR_FAIL_COND_V_MSG(dir->list_dir_begin(), nullptr, vformat("Failed to list %s.", a_directory_path));
	return dir;
}

//=================================================================================================

ResourceManager* ResourceManager::singleton = nullptr;

Variant ResourceManager::_get_from_preloader(const String &a_text) {
	if (singleton->has_resource(a_text.get_file().get_basename()))
		return singleton->get_resource(a_text.get_file().get_basename());
	return NULL;
}

Variant ResourceManager::_get_from_file_loader(const String& a_text) {
	String path = _get_full_path(a_text);
	if (path.is_abs_path()) {
		Variant v = FileLoader().load(path);
		if (!v) return NULL;
		return v;
	}
	return NULL;
}

Variant ResourceManager::_get_from_scene(const String &a_text) {
	if (!SceneTree::get_singleton() || !SceneTree::get_singleton()->get_current_scene()->has_node(a_text))
		return NULL;
	Node *node = SceneTree::get_singleton()->get_current_scene()->get_node(a_text);
	if (!node)
		return NULL;
	return node;
}

Variant ResourceManager::_get_from_group(const String& a_text) {
	if (!SceneTree::get_singleton()) return NULL;
	List<Node*> list;
	SceneTree::get_singleton()->get_nodes_in_group(a_text, &list);
	Array array;
	for (List<Node*>::Element* E = list.front(); E; E = E->next())
		array.append(E->get());
	if (array.empty())
		return NULL;
	return array;
}

String ResourceManager::_get_full_path(String a_text) const {
	if (a_text.begins_with("res://") || a_text.begins_with("usr://")) return a_text;
	if (a_text.get_extension() == "") return a_text;
	String setting = vformat("%s/%s", RESOURCE_MANAGER_DIRECTORY_SETTING, a_text.get_extension());
	if (ProjectSettings::get_singleton()->has_setting(setting))
		a_text = GLOBAL_GET(setting).operator String() + a_text;
	return a_text;
}

Vector<ResourceManager::fptr> ResourceManager::_setup_getters() {
	Vector<fptr> vector;
	vector.push_back(&ResourceManager::_get_from_preloader);
	vector.push_back(&ResourceManager::_get_from_file_loader);
	vector.push_back(&ResourceManager::_get_from_scene);
	vector.push_back(&ResourceManager::_get_from_group);
	return vector;
}

Variant ResourceManager::get_asset(const String& a_text) {
	for (int i = 0; i < m_getters.size(); i++) {
		Variant asset = (this->*m_getters[i])(a_text);
		if (!asset)
			continue;
		return asset;
	}
	print_raw(vformat("No getter could load resource '%s'.", a_text));
	return NULL;
}

String ResourceManager::get_directory(const String& a_text) {
	String setting = vformat("%s/%s", RESOURCE_MANAGER_DIRECTORY_SETTING, a_text);
	if (ProjectSettings::get_singleton()->has_setting(setting))
		return GLOBAL_GET(setting);
	else
		return "";
}

void ResourceManager::initialize() {
	if (ProjectSettings::get_singleton()->has_setting(RESOURCE_MANAGER_SETTING)) {
		Dictionary dict = GLOBAL_GET(RESOURCE_MANAGER_SETTING);
		for (int i = 0; i < dict.keys().size(); i++) {
			String key = dict.keys()[i];
			singleton->add_resource(key, _get_from_file_loader(dict[key]));
		}
	}
}

void ResourceManager::remove(const String& a_name) {
	if (ProjectSettings::get_singleton()->has_setting(RESOURCE_MANAGER_SETTING)) {
		Dictionary dict = ProjectSettings::get_singleton()->get_setting(RESOURCE_MANAGER_SETTING);
		for (int i = 0; i < dict.keys().size(); i++) {
			if (dict.keys()[i] == a_name) {
				dict.erase(a_name);
				break;
			}
		}
		ProjectSettings::get_singleton()->set_setting(RESOURCE_MANAGER_SETTING, dict);
	}
}

void ResourceManager::save() {
	List<StringName> list;
	singleton->get_resource_list(&list);
	Dictionary dict;
	for (List<StringName>::Element *E = list.front(); E; E = E->next())
		dict[E->get()] = singleton->get_resource(E->get())->get_path();
	ProjectSettings::get_singleton()->set_setting(RESOURCE_MANAGER_SETTING, dict);
	ProjectSettings::get_singleton()->save();
}

ResourceManager::ResourceManager() {
	singleton = this;
	m_getters = _setup_getters();
}

//=================================================================================================

String ResourceManagerEditor::_get_resource_name(const RES& a_res) {
	String basename = "Resource";
	if (a_res->get_name() != "") basename = a_res->get_name();
	else if (a_res->get_path().is_resource_file()) basename = a_res->get_path().get_file().get_basename();
	String name = basename;
	int counter = 0;
	while (m_placeholder->has_resource(name)) {
		counter++;
		name = basename + "_" + itos(counter);
	}
	return name;
}

void ResourceManagerEditor::_add_resource(const String& a_name, const RES& a_res) {
	m_undo_redo->create_action(TTR("Add Resource"));
	m_undo_redo->add_do_method(m_placeholder, "add_resource", a_name, a_res);
	m_undo_redo->add_do_method(m_placeholder, "save");
	m_undo_redo->add_undo_method(m_placeholder, "remove_resource", a_name);
	m_undo_redo->add_undo_method(m_placeholder, "remove", a_name);
	m_undo_redo->add_do_method(this, "_update_library");
	m_undo_redo->add_undo_method(this, "_update_library");
	m_undo_redo->commit_action();
}

void ResourceManagerEditor::_edit_resource(const String& a_old, const String& a_new, const RES& a_res) {
	m_undo_redo->create_action(TTR("Rename Resource"));
	m_undo_redo->add_do_method(m_placeholder, "remove_resource", a_old);
	m_undo_redo->add_do_method(m_placeholder, "add_resource", a_new, a_res);
	m_undo_redo->add_do_method(m_placeholder, "save");
	m_undo_redo->add_undo_method(m_placeholder, "remove_resource", a_new);
	m_undo_redo->add_undo_method(m_placeholder, "add_resource", a_old, a_res);
	m_undo_redo->add_do_method(m_placeholder, "save");
	m_undo_redo->add_do_method(this, "_update_library");
	m_undo_redo->add_undo_method(this, "_update_library");
	m_undo_redo->commit_action();
}

void ResourceManagerEditor::_remove_resource(const String& a_name, const RES& a_res) {
	m_undo_redo->create_action(TTR("Delete Resource"));
	m_undo_redo->add_do_method(m_placeholder, "remove_resource", a_name);
	m_undo_redo->add_do_method(m_placeholder, "remove", a_name);
	m_undo_redo->add_undo_method(m_placeholder, "add_resource", a_name, a_res);
	m_undo_redo->add_do_method(m_placeholder, "save");
	m_undo_redo->add_do_method(this, "_update_library");
	m_undo_redo->add_undo_method(this, "_update_library");
	m_undo_redo->commit_action();
}

void ResourceManagerEditor::_cell_button_pressed(Object* p_item, int a_column, int a_id) {
	TreeItem* item = Object::cast_to<TreeItem>(p_item);
	ERR_FAIL_COND(!item);
	switch (a_id) {
	case BUTTON_OPEN_SCENE:
		EditorInterface::get_singleton()->open_scene_from_path(item->get_text(a_column));
		break;
	case BUTTON_EDIT_RESOURCE:
		EditorInterface::get_singleton()->edit_resource(m_placeholder->get_resource(item->get_text(0)));
		break;
	case BUTTON_REMOVE:
		_remove_resource(item->get_text(0), m_placeholder->get_resource(item->get_text(0)));
		break;
	}
}

void ResourceManagerEditor::_files_load_request(const Vector<String>& a_paths) {
	for (int i = 0; i < a_paths.size(); i++) {
		String path = a_paths[i];
		RES resource;
		resource = ResourceLoader::load(path);
		if (resource.is_null()) return;
		_add_resource(_get_resource_name(resource), resource);
	}
}

void ResourceManagerEditor::_item_edited() {
	TreeItem *item = m_tree->get_selected();
	if (m_tree->get_selected_column() == 0) {
		String old_name = item->get_metadata(0);
		String new_name = item->get_text(0);
		if (new_name == "" || new_name.find("\\") != -1 || new_name.find("/") != -1 || m_placeholder->has_resource(new_name)) {
			item->set_text(0, old_name);
			return;
		}
		_edit_resource(old_name, new_name, m_placeholder->get_resource(old_name));
	}
}

void ResourceManagerEditor::_update_library() {
	m_tree->clear();
	m_tree->set_hide_root(true);
	List<StringName> rnames;
	m_placeholder->get_resource_list(&rnames);
	List<String> names;
	for (List<StringName>::Element* E = rnames.front(); E; E = E->next())
		names.push_back(E->get());
	names.sort();
	_update_tree(names);
}

void ResourceManagerEditor::_update_tree(List<String>& r_names) {
	TreeItem *root = m_tree->create_item(nullptr);
	for (List<String>::Element *E = r_names.front(); E; E = E->next()) {
		TreeItem *item = m_tree->create_item(root);
		_update_tree_create_label(item, E->get());
		RES res = m_placeholder->get_resource(E->get());
		ERR_CONTINUE(res.is_null());
		String type = res->get_class();
		_update_tree_create_buttons(item, type, res);
	}
}

void ResourceManagerEditor::_update_tree_create_label(TreeItem* p_ti, const String& a_name) {
	p_ti->set_cell_mode(0, TreeItem::CELL_MODE_STRING);
	p_ti->set_editable(0, true);
	p_ti->set_selectable(0, true);
	p_ti->set_text(0, a_name);
	p_ti->set_metadata(0, a_name);
}

void ResourceManagerEditor::_update_tree_create_buttons(TreeItem* p_ti, const String& a_type, const RES& a_res) {
	p_ti->set_icon(0, EditorNode::get_singleton()->get_class_icon(a_type, "Object"));
	p_ti->set_tooltip(0, TTR("Instance:") + " " + a_res->get_path() + "\n" + TTR("Type:") + " " + a_type);
	p_ti->set_text(1, a_res->get_path());
	p_ti->set_editable(1, false);
	p_ti->set_selectable(1, false);
	if (a_type == "PackedScene")
		p_ti->add_button(1, get_icon("InstanceOptions", "EditorIcons"), BUTTON_OPEN_SCENE, false, TTR("Open in Editor"));
	else
		p_ti->add_button(1, get_icon("Load", "EditorIcons"), BUTTON_EDIT_RESOURCE, false, TTR("Open in Editor"));
	p_ti->add_button(1, get_icon("Remove", "EditorIcons"), BUTTON_REMOVE, false, TTR("Remove"));
}

Variant ResourceManagerEditor::_get_drag_data_fw(const Point2& a_point, Control* p_from) {
	TreeItem *ti = m_tree->get_item_at_position(a_point);
	if (!ti) return Variant();
	RES res = m_placeholder->get_resource(ti->get_metadata(0));
	if (!res.is_valid()) return Variant();
	return EditorNode::get_singleton()->drag_resource(res, p_from);
}

bool ResourceManagerEditor::_can_drop_data_fw(const Point2& a_point, const Variant& a_data, Control* p_from) const {
	Dictionary dict = a_data;
	if (!dict.has("type") || (dict.has("from") && (Object *)(dict["from"]) == m_tree))
		return false;
	if (String(dict["type"]) == "resource" && dict.has("resource"))
		return ((RES)dict["resource"]).is_valid();
	if (String(dict["type"]) == "files")
		return ((Vector<String>)dict["files"]).size() != 0;
	return false;
}

void ResourceManagerEditor::_drop_data_fw(const Point2& a_point, const Variant& a_data, Control* p_from) {
	Dictionary dict = a_data;
	if (!_can_drop_data_fw(a_point, a_data, p_from) || !dict.has("type"))
		return;
	if (String(dict["type"]) == "resource" && dict.has("resource")) {
		RES res = dict["resource"];
		if (res.is_valid()) _add_resource(_get_resource_name(res), res);
	}
	if (String(dict["type"]) == "files")
		_files_load_request(((Vector<String>)dict["files"]));
}

void ResourceManagerEditor::edit() {
	m_placeholder = memnew(ResourceManager);
	m_placeholder->initialize();
	_update_library();
}

ResourceManagerEditor::ResourceManagerEditor() {
	VBoxContainer* vbc = memnew(VBoxContainer);
	add_child(vbc);
	_initialize_tree();
	vbc->add_child(m_tree);
}

void ResourceManagerEditor::_initialize_tree() {
	m_tree = memnew(Tree);
	m_tree->connect("button_pressed", this, "_cell_button_pressed");
	m_tree->set_columns(2);
	m_tree->set_column_min_width(0, 2);
	m_tree->set_column_min_width(1, 3);
	m_tree->set_column_expand(0, true);
	m_tree->set_column_expand(1, true);
	m_tree->set_v_size_flags(SIZE_EXPAND_FILL);
	m_tree->set_drag_forwarding(this);
	m_tree->connect("item_edited", this, "_item_edited");
}

//=================================================================================================

void ResourceManagerPlugin::_notification(int a_what) {
	if (a_what == NOTIFICATION_ENTER_TREE)
		_on_enter_tree();
}

void ResourceManagerPlugin::_on_enter_tree() {
	TabContainer *dock = (TabContainer *)m_manager_editor->get_parent_control();
	dock->set_tab_title(m_manager_editor->get_index(), TTR("Resource"));
	m_manager_editor->set_undo_redo(&get_undo_redo());
	m_manager_editor->edit();
}

ResourceManagerPlugin::ResourceManagerPlugin(EditorNode* p_node) {
	m_manager_editor = memnew(ResourceManagerEditor);
	m_manager_editor->set_custom_minimum_size(Size2(0, 250) * EDSCALE);
	p_node->add_control_to_dock(EditorNode::DOCK_SLOT_LEFT_UR, m_manager_editor);
}
