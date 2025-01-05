#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Renderwindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/window.hpp>

sf::Font font;

sf::RenderWindow* window = nullptr;

/*---------------------------------------------------
    Drug pharmacy logic
  ---------------------------------------------------*/

bool is_number(std::string& input)
{
    if (input.empty())
        return false;
    for (char c : input)
        if (!std::isdigit(c) && c != '.')
            return false;
    return true;
}

struct List_node
{
    std::string label;
    int Qte, id;
    List_node* next;
};

class List
{
private:
    List_node* head;

public:
    List()
        : head(nullptr)
    {
    }

    List_node* get_head()
    {
        return head;
    }

    void remove_product(std::string input)
    {
        bool is_num = is_number(input);

        auto [index, user] = search_user(head, input, !is_num);
        if (user != nullptr)
        {  // if the user is found
            user = head;
            if (index > 0)
            {  // check if the user is the one on the list
                for (int i = 1; i < index; i++)
                {  // grab the previous user
                    user = user->next;
                }
                std::cout << "successfully deleted: " << user->next->label << "\n";
                user->next = user->next->next;  // change the user's adress to the next user's
                                                // adress to leave the current one flying in the
                                                // skies of ram!!!!!
            }
            else
            {
                std::cout << "successfully deleted: " << user->label << "\n";
                head = head->next;  // if the user is the first element, we edit the head of the
                                    // list
            }
        }
    }

    std::pair<int, List_node*> search_user(List_node* list, std::string input, bool is_by_name)
    {
        int idx = 0;
        if (is_by_name)
        {
            while (list != nullptr)
            {
                if ((*list).label == input)
                    return { idx, list };
                list = list->next;
                idx++;
            }
            return { 0, nullptr };
        }
        else
        {
            if (is_number(input))
            {
                while (list != nullptr)
                {
                    if ((*list).id == stoi(input))
                        return { idx, list };
                    list = list->next;
                    idx++;
                }
            }
            return { 0, nullptr };
        }
    }

    void add_product(std::string label, int quantity, int id)
    {
        List_node* new_node = new List_node;

        new_node->label = label;
        new_node->Qte = quantity;
        new_node->id = id;

        (*new_node).next = head;
        head = new_node;
    }

    void print_list(List_node* list, std::string print_type, std::string& line)
    {
        if (print_type == "label")
        {
            while (list != nullptr)
            {
                line += "-" + list->label + "\n";
                list = (*list).next;
            }
        }
        else if (print_type == "all")
        {
            while (list != nullptr)
            {
                line += "-" + list->label + "(#" + std::to_string(list->id) +
                        "): " + std::to_string(list->Qte) + " mg\n";
                list = (*list).next;
            }
        }
    }

    void replace_products_in_list(List_node* node, std::string label, std::string n_label, int n_id,
                                  int n_Qte)
    {
        while (node != nullptr)
        {
            if (node->label == label)
            {
                node->label = n_label;
                node->id = n_id;
                node->Qte = n_Qte;
            }
            node = node->next;
        }
    }
};

struct Tree_node
{
    int ID{};
    std::string Label{};
    float price{};
    std::string category{};
    Tree_node* left{};
    Tree_node* right{};
    int x{}, y{};
    List product_list{};

    Tree_node(int id, const std::string& label, float p, const std::string& cat)
        : ID(id)
        , Label(label)
        , price(p)
        , category(cat)
        , left(nullptr)
        , right(nullptr)
    {
    }
};

class Tree
{
private:
    Tree_node* root;

    Tree_node* insert_node(Tree_node* node, int id, const std::string& label, float price,
                           const std::string& category)
    {
        if (node == nullptr)
            return new Tree_node(id, label, price, category);

        // binary tree search
        if (id < node->ID)
            node->left = insert_node(node->left, id, label, price, category);
        else if (id > node->ID)
            node->right = insert_node(node->right, id, label, price, category);
        // if the id is equal dont add
        return node;
    }

    void removeDuplicates(Tree_node* node, std::unordered_set<std::string>& seen,
                          std::vector<std::string>& result)
    {
        if (!node)
            return;  // Base case: Null node

        // Inorder traversal: Left -> Current -> Right
        removeDuplicates(node->left, seen, result);

        // Process the current node
        if (seen.find(node->category) == seen.end())
        {
            seen.insert(node->category);       // Mark as seen
            result.push_back(node->category);  // Add to the result
        }

        removeDuplicates(node->right, seen, result);
    }

    void display_drug_on_category(Tree_node* node, const std::string& category)
    {
        if (node)
        {
            if (node->category == category)
                std::cout << "ID: " << node->ID << " Label: " << node->Label
                          << " Price: " << node->price << "|---|";
            display_drug_on_category(node->left, category);
            display_drug_on_category(node->right, category);
        }
    }

public:
    Tree()
        : root(nullptr)
    {
    }  // Constructor

    void add_node(int id, const std::string& label, float price, const std::string& category)
    {
        root = insert_node(root, id, label, price, category);
    }

    void display_tree(Tree_node* node, int level)
    {
        if (node != nullptr)
        {
            for (int i = 0; i < level; i++)
                std::cout << "        ";
            std::cout << "|------>|";

            std::cout << "Drug category: " << node->category << ", Drug label: " << (*node).Label
                      << ", Drug price: " << node->price << "$, drug id: " << node->ID << "\n";
            display_tree((*node).left, level + 1);
            display_tree((*node).right, level + 1);
        }
        else
        {
            std::cout << "\n";
        }
    }

    Tree_node* get_root()
    {
        return root;
    }

    Tree_node* validate(Tree_node* node, int id)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        else
        {
            if (id == node->ID)
                return node;
            if (id > node->ID)
                return validate(node->right, id);
            else
                return validate(node->left, id);
        }
    }

    void display_labels(Tree_node* node, std::string& text)
    {
        if (node != nullptr)
        {
            std::string price_string = std::to_string(node->price);

            for (int i = 1; i < 5; i++)
            {  // removing the trailing zeroes
                price_string.pop_back();
            }

            text += "-" + node->Label + "(#" + std::to_string(node->ID) + "): " + price_string +
                    " $/g. [" + node->category + "]\n";

            display_labels(node->left, text);
            display_labels(node->right, text);
        }
    }

    void display_categories()
    {
        if (root != nullptr)
        {
            std::unordered_set<std::string> seen;
            std::vector<std::string> result;

            removeDuplicates(root, seen, result);
            std::cout << "categories:\n";

            for (std::string& category : result)
                std::cout << "-" << category << ".\n";
        }
    }

    void display_tree_categories_based()
    {
        std::unordered_set<std::string> seen;
        std::vector<std::string> result;

        removeDuplicates(root, seen, result);
        for (std::string& category : result)
        {
            std::cout << category << ":\n";
            display_drug_on_category(root, category);
            std::cout << "\n";
        }
    }

    void replace_products(Tree_node* node, std::string p_label, std::string n_label, int n_id,
                          int n_Qte)
    {
        if (node != nullptr)
        {
            node->product_list.replace_products_in_list(node->product_list.get_head(), p_label,
                                                        n_label, n_id, n_Qte);
            replace_products(node->left, p_label, n_label, n_id, n_Qte);
            replace_products(node->right, p_label, n_label, n_id, n_Qte);
        }
    }
};

// old implementation for drug addition
/*void input_new_drug(Tree& tree){
    std::string label="0", category="0", id="f", price="f";

    std::cout<<"============New Drug============\n";

    while(is_number(label)){
    std::cout<<"insert product label:";
    std::getline(std::cin, label);
    if(is_number(label)){std::cout<<"ERROR: label cannot be a number! try again.\n";}
    }

    while(is_number(category)){
    std::cout<<"insert product category:";
    std::getline(std::cin, category);
    if(is_number(category)){std::cout<<"ERROR: category cannot be a number! try again.\n";}
    }

    while(!is_number(id)){
    std::cout<<"insert product id:";
    std::getline(std::cin, id);
    if(!is_number(id)){std::cout<<"ERROR: id must be a number! try again.\n";}
    }

    while(!is_number(price)){
    std::cout<<"insert product price:";
    std::getline(std::cin, price);
    if(!is_number(price)){std::cout<<"ERROR: price must be a number! try again.\n";}
    }

    tree.add_node(stoi(id), label, stof(price), category);

    std::cout<<"successfully added the new drug!\n";
}*/

Tree_node* min_node(Tree_node* node)
{
    while (node->left != nullptr)
        node = node->left;
    return node;
}

Tree_node* Delet_drug(Tree_node* node, int ID)
{
    if (node == nullptr)
        return node;  // Base case: Node not found

    if (ID < node->ID)
    {
        node->left = Delet_drug(node->left, ID);
    }
    else if (ID > node->ID)
    {
        node->right = Delet_drug(node->right, ID);
    }
    else if (node->left == nullptr && node->right == nullptr)
    {
        // Case 1: No children (leaf node)
        delete node;
        return nullptr;
    }
    else if (node->left == nullptr)
    {
        // Case 2: Only right child
        Tree_node* temp = node->right;
        delete node;
        return temp;
    }
    else if (node->right == nullptr)
    {
        // Case 3: Only left child
        Tree_node* temp = node->left;
        delete node;
        return temp;
    }
    else
    {
        // Case 4: Two children
        Tree_node* successor = min_node(node->right);
        node->ID = successor->ID;
        node->Label = successor->Label;
        node->price = successor->price;
        node->category = successor->category;
        node->product_list = successor->product_list;
        // Remove the successor from the right subtree
        node->right = Delet_drug(node->right, successor->ID);
    }
    return node;
}

void Delet_drug_category(Tree tree, Tree_node* node, std::string category)
{
    if (node == nullptr)
    {
        return;
    }
    else
    {
        Delet_drug_category(tree, node->left, category);
        Delet_drug_category(tree, node->right, category);
        if (node->category == category)
            Delet_drug(tree.get_root(), node->ID);
    }
}

void display_product_drugs(Tree_node* node, std::string label, bool is_by_name, std::string& text)
{
    if (node != nullptr)
    {
        auto [idx, t] = node->product_list.search_user(node->product_list.get_head(), label,
                                                       is_by_name);
        if (t != nullptr)
        {
            std::string price_str = std::to_string(node->price);
            for (int i = 1; i < 5; i++)
                price_str.pop_back();

            text += "-" + node->Label + "(#" + std::to_string(node->ID) + "): " + price_str +
                    "$ [" + node->category + "]\n";
        }
        display_product_drugs(node->left, label, is_by_name, text);
        display_product_drugs(node->right, label, is_by_name, text);
    }
}

void format_tree(Tree_node* node, std::ofstream& file)
{
    // format: #label#id#price#category&#label#qte#id#next&#label#qte#id#next
    //& indicates a list member

    if (node != nullptr)
    {
        file << "^#" << node->Label << "#" << node->ID << "#" << node->price << "#"
             << node->category;
        List_node* list_node = node->product_list.get_head();
        while (list_node != nullptr)
        {
            file << "&#" << list_node->label << "#" << list_node->Qte << "#" << list_node->id;
            list_node = list_node->next;
        }
        file << "\n";

        format_tree(node->left, file);
        format_tree(node->right, file);
    }
}

void save_tree(Tree_node* root, std::string file_name)
{
    std::ofstream file(file_name);
    if (!file.is_open())
    {
        std::cout << "Unable to open file\n";
        return;
    }
    else
    {
        std::cout << "file is open! ready to write...\n";
    }
    format_tree(root, file);
    file.close();
}

int load_node(std::string line, size_t i, Tree& tree)
{
    char c;
    std::string token, label, category;
    int id;
    float price;

    while (i < line.size())
    {  // load label
        c = line[i];
        if (c == '#')
        {
            i++;
            break;
        }
        token += line[i];
        i++;
    }
    label = token;
    token = "";
    while (i < line.size())
    {  // load id
        c = line[i];
        if (c == '#')
        {
            i++;
            break;
        }
        token += line[i];
        i++;
    }
    id = stoi(token);
    token = "";

    while (i < line.size())
    {  // load price
        c = line[i];
        if (c == '#')
        {
            i++;
            break;
        }
        token += line[i];
        i++;
    }
    price = stof(token);
    token = "";

    while (i < line.size())
    {  // load category
        c = line[i];
        if (c == '#' || c == '&')
        {
            i++;
            break;
        }
        token += line[i];
        i++;
    }
    category = token;
    token = "";

    tree.add_node(id, label, price, category);
    return id;
}

void load_products(std::string line, size_t i, Tree tree, int node_id)
{
    Tree_node* node = tree.validate(tree.get_root(), node_id);
    char c;
    std::string label, token;
    int id, Qte;

    while (i < line.size())
    {
        c = line[i];
        if (c == '#')
        {
            i++;
            break;
        }
        token += c;
        i++;
    }
    label = token;
    token = "";

    while (i < line.size())
    {
        c = line[i];
        if (c == '#')
        {
            i++;
            break;
        }
        token += c;
        i++;
    }
    Qte = stoi(token);
    token = "";

    while (i < line.size())
    {
        c = line[i];
        if (c == '#')
        {
            i++;
            break;
        }
        token += c;
        i++;
    }
    id = stoi(token);
    token = "";

    node->product_list.add_product(label, Qte, id);
}

void load_tree(std::string file_name, Tree& tree)
{
    std::ifstream file(file_name);

    if (!file.is_open())
    {
        std::cerr << "Unable to open file\n";
        return;
    }

    std::string line;
    size_t i = 0;
    int current_node_id;
    char c;
    // main reading loop:
    while (std::getline(file, line))
    {
        i = 0;
        while (i < line.size())
        {
            c = line[i];
            if (c == '^')
                current_node_id = load_node(line, i + 2, tree);
            if (c == '&')
                load_products(line, i + 2, tree, current_node_id);
            i++;
        }
    }
    file.close();
}

void load_defaults(Tree& tree)
{
    tree.add_node(5, "dopamine", 11.50, "happiness");  // initial testing sample
    tree.add_node(8, "serotonin", 12.50, "happiness");
    tree.add_node(2, "endorphin", 13.50, "enderness");
    tree.add_node(3, "kotakonin", 1.50, "lolliness");
}

Tree product_tree1;

/*---------------------------------------------------
    USER INTERFACE logic

    Button class - textures
  ---------------------------------------------------*/

class TextureManager
{
private:
    std::map<std::string, sf::Texture> textures;

public:
    void load(const std::string& name, const std::string& filepath)
    {
        sf::Texture texture;
        if (!texture.loadFromFile(filepath))
            throw std::runtime_error("Failed to load texture: " + filepath);
        textures[name] = std::move(texture);
    }

    sf::Texture& get(const std::string& name)
    {
        return textures.at(name);
    }
};

// Global Texture Manager
TextureManager textureManager;

void initializeTextures()
{
    // Get the executable's directory
    std::string assetsPath = "assets/icons/";

    // Load textures
    textureManager.load("add.png", assetsPath + "add.png");
    textureManager.load("back.png", assetsPath + "back.png");
    textureManager.load("edit.png", assetsPath + "edit.png");
    textureManager.load("delete.png", assetsPath + "delete.png");
    textureManager.load("logout.png", assetsPath + "logout.png");
    textureManager.load("replace.png", assetsPath + "replace.png");
    textureManager.load("save.png", assetsPath + "save.png");
    textureManager.load("loading.png", assetsPath + "loading.png");
    textureManager.load("background.png", assetsPath + "background1.png");
}

class Button
{
public:
    Button(float x, float y, float width, float height, const sf::Font& font,
           const std::string& text, bool active, int id, const std::string& icon_name)
        : s_active(active)
        , s_id(id)
        , isHovered(false)
    {
        m_button.setSize(sf::Vector2f(width, height));
        m_button.setPosition(x, y);
        m_button.setFillColor(sf::Color::Blue);
        assign_icon(icon_name, x, y, width);

        m_text.setFont(font);
        m_text.setString(text);
        m_text.setCharacterSize(20);
        m_text.setFillColor(sf::Color::Black);
        m_text.setPosition(x + 10, y + height / 4);  // Adjust text position
    }

    void addAssociatedField(int textFieldId)
    {
        associatedFields.push_back(textFieldId);
    }

    const std::vector<int>& getAssociatedFields() const
    {
        return associatedFields;
    }

    void updateHover(const sf::Vector2i& mousePos)
    {
        if (m_button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
            isHovered = true;
        else
            isHovered = false;
    }

    void draw(sf::RenderWindow* window)
    {
        if (isHovered && s_active)
        {
            m_button.setFillColor(sf::Color(204, 137, 91));  // Light grey when hovered
            m_button.setOutlineThickness(2);                 // Subtle outline
            m_button.setOutlineColor(sf::Color::Black);      // Slightly darker grey outline
        }
        else if (s_active)
        {
            m_button.setFillColor(sf::Color(220, 157, 111));  // White when active
            m_button.setOutlineThickness(1);                  // Thin grey outline
            m_button.setOutlineColor(sf::Color::Black);       // Light grey
        }
        else
        {
            m_button.setFillColor(sf::Color(245, 245, 245));  // Very light grey when inactive
            m_button.setOutlineThickness(1);                  // Thin outline
            m_button.setOutlineColor(sf::Color::Black);       // Light grey
        }

        window->draw(m_button);
        window->draw(m_text);
        window->draw(icon_sprite);
    }

    bool isClicked(const sf::Vector2i& mousePos)
    {
        return m_button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

    bool isHoveredOver() const
    {
        return isHovered;
    }

    int getId() const
    {
        return s_id;
    }

private:
    sf::RectangleShape m_button;
    sf::Text m_text;
    sf::Texture icon;
    sf::Sprite icon_sprite;
    bool s_active;
    int s_id;
    bool isHovered;
    std::vector<int> associatedFields;  // List of associated text input field IDs

    void assign_icon(const std::string& icon_name, float x, float y, float width)
    {
        icon = textureManager.get(icon_name);
        icon_sprite.setTexture(icon);
        icon_sprite.setPosition(x + width - 25, y);
        icon_sprite.setScale(0.05f, 0.05f);
    }
};

std::vector<Button> buttons;

void drawButtons()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    for (Button& button : buttons)
    {
        button.updateHover(mousePos);
        button.draw(window);
    }
}

void draw_background()
{
    // Retrieve the background texture from the texture manager
    const sf::Texture& background = textureManager.get("background.png");

    sf::Sprite background_sprite;
    background_sprite.setTexture(background);

    // Get texture and window sizes
    sf::Vector2u textureSize = background.getSize();
    sf::Vector2u windowSize = window->getSize();

    // Calculate scaling factors to fit the texture to the window
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    // Apply the scale to the sprite
    background_sprite.setScale(scaleX, scaleY);

    // Draw the background to the window
    window->draw(background_sprite);
}

int handleClick(const sf::Vector2i& mousePos)
{
    for (Button& button : buttons)
        if (button.isClicked(mousePos))
            return button.getId();  // Return the ID of the clicked button
    return -1;                      // No button clicked
}

/*---------------------------------------------------
    USER INTERFACE logic

    Text class
   ---------------------------------------------------*/

class TextInputField
{
public:
    TextInputField(float x, float y, float width, float height, const sf::Font& font,
                   const std::string& placeholder = "")
    {
        m_box.setSize(sf::Vector2f(width, height));
        m_box.setPosition(x, y);
        m_box.setFillColor({ 254, 187, 141 });
        m_box.setOutlineColor(sf::Color::Black);
        m_box.setOutlineThickness(2);

        m_text.setFont(font);
        m_text.setString(placeholder);
        m_text.setCharacterSize(20);
        m_text.setFillColor(sf::Color(80, 80, 80));  // Gray for placeholder
        m_text.setPosition(x + 5, y + (height - m_text.getCharacterSize()) / 2);

        m_placeholder = placeholder;
        m_isFocused = false;
    }

    void draw(sf::RenderWindow* window)
    {
        window->draw(m_box);
        window->draw(m_text);
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow* window)
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            // Check if the text box is clicked
            if (m_box.getGlobalBounds().contains(
                    static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window))))
            {
                m_isFocused = true;
                m_text.setFillColor(sf::Color::Black);  // Black for active input
                if (m_text.getString() == m_placeholder)
                    m_text.setString("");  // Clear placeholder on focus
            }
            else
            {
                m_isFocused = false;
                if (m_text.getString().isEmpty())
                {
                    m_text.setString(m_placeholder);  // Restore placeholder
                    m_text.setFillColor(sf::Color(80, 80, 80));
                }
            }
        }

        if (m_isFocused && event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == '\b')
            {  // Handle backspace
                if (!m_input.empty())
                    m_input.pop_back();
            }
            else if (event.text.unicode < 128 && event.text.unicode != '\b')
            {
                m_input += static_cast<char>(event.text.unicode);
            }
            m_text.setString(m_input);
        }
    }

    void highlight(bool isHighlighted)
    {
        if (isHighlighted)
        {
            m_box.setOutlineColor(sf::Color(105, 40, 0));  // Highlighted outline
            m_box.setOutlineThickness(4);                  // Thicker outline
        }
        else
        {
            m_box.setOutlineColor(sf::Color::Black);  // Normal outline
            m_box.setOutlineThickness(2);
        }
    }

    std::string getInput() const
    {
        return m_input;
    }

    void setInput(const std::string& input)
    {
        m_input = input;
        m_text.setString(m_input);
        m_text.setFillColor(sf::Color::Black);
    }

private:
    sf::RectangleShape m_box;   // The input box
    sf::Text m_text;            // The text inside the box
    std::string m_input;        // The user's input
    std::string m_placeholder;  // Placeholder text
    bool m_isFocused;           // Focus state
    // int m_id;
};

std::vector<TextInputField> Text_zones;

void initTextZones(std::vector<std::string>& Text_zone_labels, int sx, int sy)
{
    Text_zones.clear();
    float x = 270, y = 80;

    for (const std::string& label : Text_zone_labels)
    {
        Text_zones.emplace_back(x, y, sx, sy, font, label);
        y += sy + 10;
    }
}

void drawButtons_texts(std::vector<TextInputField>& textFields)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

    for (TextInputField& field : textFields)
        field.highlight(false);

    for (Button& button : buttons)
    {  // highlight mechanism
        button.updateHover(mousePos);
        if (button.isHoveredOver())
        {
            for (int id : button.getAssociatedFields())
                if (id > 0 && id <= static_cast<int>(textFields.size()))
                    textFields[id - 1].highlight(true);
        }
        button.draw(window);
    }

    for (TextInputField& field : textFields)
        field.draw(window);
}

void initButtons(const std::vector<std::string>& Button_labels, int sx, int sy, int mode,
                 const std::vector<std::string>& icon_labels)
{
    buttons.clear();
    float x = 10, y = 80;
    int id = 1;

    for (const std::string& label : Button_labels)
    {
        id++;
        buttons.emplace_back(x, y, sx, sy, font, label, true, id - 1, icon_labels[id]);
        y += sy + 10;
    }

    switch (mode)
    {
        case 1:
            // add drug button
            for (size_t i = 1; i < Text_zones.size(); i++)
                buttons[0].addAssociatedField(i);
            // edit drug button
            buttons[1].addAssociatedField(3);
            // Replace drug containing a product
            for (size_t i = 1; i < Text_zones.size(); i++)
                buttons[2].addAssociatedField(i);
            // delet drug button
            buttons[3].addAssociatedField(3);
            // delet category button
            buttons[4].addAssociatedField(2);
            // save button
            buttons[5].addAssociatedField(5);
            break;
        case 2:
            buttons[0].addAssociatedField(1);  // label
            buttons[1].addAssociatedField(2);  // price
            buttons[2].addAssociatedField(3);  // category
            buttons[3].addAssociatedField(4);  // id
            break;
        case 3:
            // add button
            for (size_t i = 1; i <= Text_zones.size(); i++)
                buttons[0].addAssociatedField(i);  // all
            // remove button
            buttons[1].addAssociatedField(1);  // label
            break;
    }
}

void calc_positions(Tree_node* node, int x, int y, int H_spacing)
{
    if (!node)
        return;
    // Set the node's position
    node->x = x;
    node->y = y;

    // Reduce horizontal spacing as we go deeper
    int childSpacing = H_spacing / 2;

    if (node->left != nullptr)
        calc_positions(node->left, x - childSpacing, y + 70, childSpacing);
    if (node->right != nullptr)
        calc_positions(node->right, x + childSpacing, y + 70, childSpacing);
}

void drawTree(Tree_node* node)
{
    if (!node)
        return;

    // Draw lines to children
    if (node->left)
    {
        sf::Vertex line[] = { sf::Vertex(sf::Vector2f(node->x, node->y), sf::Color::White),
                              sf::Vertex(sf::Vector2f(node->left->x, node->left->y),
                                         sf::Color::White) };
        window->draw(line, 2, sf::Lines);
    }

    if (node->right)
    {
        sf::Vertex line[] = { sf::Vertex(sf::Vector2f(node->x, node->y), sf::Color::White),
                              sf::Vertex(sf::Vector2f(node->right->x, node->right->y),
                                         sf::Color::White) };
        window->draw(line, 2, sf::Lines);
    }

    // Draw the node as a circle
    sf::CircleShape circle(20);
    if (node->left == nullptr && node->right == nullptr)
        circle.setFillColor(sf::Color(50, 150, 50));
    else
        circle.setFillColor(sf::Color(105, 40, 0));

    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2);
    circle.setPosition(node->x - 20, node->y - 20);  // Center the circle
    window->draw(circle);

    // Draw the key inside the node
    sf::Text text;
    text.setFont(font);
    text.setString(node->Label + "(#" + std::to_string(node->ID) + ")");
    text.setCharacterSize(17);
    text.setFillColor(sf::Color::White);
    text.setPosition(node->x - 40, node->y - 45);  // Center the text
    window->draw(text);

    // Recurse for children
    drawTree(node->left);
    drawTree(node->right);
}

void initializeText(sf::Text& promt, sf::Vector2f position, int size, sf::Color color,
                    std::string input)
{
    promt.setFont(font);
    promt.setCharacterSize(size);
    promt.setFillColor(color);
    promt.setPosition(position);
    promt.setString(input);
}

void Draw_Gui_3(List list, std::string label)
{
    sf::Text List_header, List_content, menu;

    std::string List_header_string = "Products inside the drug:",
                menu_string = "Drug Product manager: " + label, List_content_string;
    list.print_list(list.get_head(), "all", List_content_string);

    initializeText(menu, { 35, 15 }, 42, { 40, 40, 40 }, menu_string);
    initializeText(List_header, { 660, 90 }, 35, sf::Color::White, List_header_string);
    initializeText(List_content, { 670, 130 }, 30, { 230, 230, 230 }, List_content_string);

    sf::RectangleShape box;
    box.setSize({ 450, 700 });
    box.setPosition({ 650, 75 });
    box.setFillColor({ 40, 40, 40, 165 });
    box.setOutlineColor({ sf::Color::Black });
    box.setOutlineThickness(2);

    draw_background();
    drawButtons_texts(Text_zones);

    window->draw(box);
    window->draw(menu);
    window->draw(List_header);
    window->draw(List_content);
}

void menu3(Tree_node* drug)
{
    std::vector<std::string> Button_labels = { "Add Product", "Remove Product", "_", "Back to Main Menu" };
    std::vector<std::string> icon_labels = { "add.png", "delete.png", "loading.png", "back.png" };
    initButtons(Button_labels, 250, 50, 3, icon_labels);  // Initialize the buttons with menu3 options
    std::vector<std::string> Text_field_labels = { "Product label...", "Product ID...", "Product Quantity..." };
    initTextZones(Text_field_labels, 350, 50);
    std::string product_label, product_id, product_qte;

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                int clickedId = handleClick(sf::Mouse::getPosition(*window));

                // get the data in the input buffers
                product_label = Text_zones[0].getInput();
                product_id = Text_zones[1].getInput();
                product_qte = Text_zones[2].getInput();
                if (clickedId != -1)
                {
                    switch (clickedId)
                    {
                        case 1:
                            if (is_number(product_id) && is_number(product_qte) &&
                                product_label != "")
                            {
                                drug->product_list.add_product(product_label, stoi(product_qte),
                                                               stoi(product_id));
                                for (TextInputField& zone : Text_zones)
                                    zone.setInput("Success!");
                            }
                            else
                            {
                                if (!is_number(product_id))
                                    Text_zones[1].setInput("Invalid input.");
                                if (!is_number(product_qte))
                                    Text_zones[2].setInput("Invalid input.");
                            }
                            break;
                        case 2:
                            drug->product_list.remove_product(product_label);
                            break;
                        case 3:
                            break;
                        case 4:
                            return;  // Return to the previous menu
                    }
                }
            }
            for (TextInputField& inputField : Text_zones)
                inputField.handleEvent(event, window);
        }

        window->clear(sf::Color::Black);
        Draw_Gui_3(drug->product_list, drug->Label);
        window->display();
    }
}

void Draw_Gui_2(Tree_node* drug)
{
    std::string current_drug_string = "Current Drug: " + drug->Label + " | " +
                                      "Category: " + drug->category + " | " +
                                      "Price: " + std::to_string(drug->price) + "$ | " +
                                      "ID: " + std::to_string(drug->ID);
    std::string drug_product_string, menu_string = "Drug editor:";
    drug->product_list.print_list(drug->product_list.get_head(), "all", drug_product_string);

    sf::Text current_drug_text, drug_products, menu;
    initializeText(menu, { 15, 12.5 }, 42, { 40, 40, 40 }, menu_string);

    initializeText(current_drug_text, { 550, 70 }, 20, sf::Color::White, current_drug_string);
    initializeText(drug_products, { 560, 100 }, 25, sf::Color::White, drug_product_string);

    sf::RectangleShape box;
    box.setSize({ 655, 320 });
    box.setPosition({ 535, 60 });
    box.setFillColor({ 40, 40, 40, 165 });
    box.setOutlineColor({ sf::Color::Black });
    box.setOutlineThickness(2);

    draw_background();
    drawButtons_texts(Text_zones);
    window->draw(box);
    window->draw(menu);
    window->draw(current_drug_text);
    window->draw(drug_products);
}

int menu2(Tree_node* drug)
{
    // buttons data
    std::vector<std::string> Button_labels = {
        "Edit Drug Label", "Edit Drug Price", "Edit Drug Category", "Edit Drug ID", "_",
        "Edit Product Requirements", "Back to Main Menu"
    };
    std::vector<std::string> icon_labels = { "edit.png", "edit.png", "edit.png", "edit.png",
                                             "loading.png", "edit.png", "back.png" };

    initButtons(Button_labels, 250, 50, 2, icon_labels);

    // text input zones data
    std::vector<std::string> text_field_labels = { "Drug label...", "Drug price...",
                                                   "Drug category...", "Drug ID..." };
    initTextZones(text_field_labels, 250, 50);

    std::string label, category, price, id;
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                int clickedId = handleClick(sf::Mouse::getPosition(*window));

                label = Text_zones[0].getInput();
                price = Text_zones[1].getInput();
                category = Text_zones[2].getInput();
                id = Text_zones[3].getInput();
                ;

                if (clickedId != -1)
                {
                    switch (clickedId)
                    {
                        case 1:
                            if (!is_number(label) && label != "")
                            {
                                drug->Label = label;
                                Text_zones[0].setInput("");
                            }
                            else
                            {
                                Text_zones[0].setInput("Invalid input.");
                            }
                            break;
                        case 2:
                            if (is_number(price))
                            {
                                drug->price = std::stof(price);
                                Text_zones[1].setInput("");
                            }
                            else
                            {
                                Text_zones[1].setInput("Invalid input.");
                            }
                            break;
                        case 3:
                            if (!is_number(category) && category != "")
                            {
                                drug->category = category;
                                Text_zones[2].setInput("");
                            }
                            else
                            {
                                Text_zones[2].setInput("Invalid input.");
                            }
                            break;
                        case 4:
                            if (is_number(id) && stoi(id) != drug->ID)
                            {
                                product_tree1.add_node(std::stoi(id), drug->Label, drug->price,
                                                       drug->category);
                                Text_zones[3].setInput("");
                                Delet_drug(product_tree1.get_root(), drug->ID);
                                return stoi(id);
                            }
                            else
                            {
                                Text_zones[3].setInput("Error.");
                            }
                            break;
                        case 5:
                            break;
                        case 6:
                            menu3(drug);
                            initButtons(Button_labels, 250, 50, 2, icon_labels);
                            initTextZones(text_field_labels, 250, 50);
                            break;
                        case 7:
                            return -1;
                    }
                }
            }

            for (TextInputField& inputField : Text_zones)
                inputField.handleEvent(event, window);
        }

        window->clear(sf::Color::Black);
        Draw_Gui_2(drug);
        window->display();
    }
    return -1;
}

void Draw_Gui_1(std::string label)
{
    draw_background();

    sf::Text menu, drug_data, drug_header, containing_list, containing_header;

    std::string menu_string = "------Production Menu-------";
    std::string drug_header_string = "Produced drug list:";
    std::string drug_data_string;
    std::string containing_header_string = "Drugs containing " + label + ":";
    std::string drug_containing;

    display_product_drugs(product_tree1.get_root(), label, !is_number(label),
                          drug_containing);                                    // getting the drug list containing a product
    product_tree1.display_labels(product_tree1.get_root(), drug_data_string);  // getting the drug
                                                                               // list

    initializeText(menu, { 20, 20 }, 40, { 40, 40, 40 }, menu_string);
    initializeText(drug_data, { 710, 80 }, 25, { 240, 240, 240 }, drug_data_string);
    initializeText(drug_header, { 700, 35 }, 30, { 240, 240, 240 }, drug_header_string);
    initializeText(containing_list, { 710, 445 }, 25, { 240, 240, 240 }, drug_containing);
    initializeText(containing_header, { 700, 400 }, 30, { 240, 240, 240 }, containing_header_string);

    sf::RectangleShape box;
    box.setSize({ 505, 330 });
    box.setPosition({ 685, 35 });
    box.setFillColor({ 40, 40, 40, 165 });
    box.setOutlineColor({ sf::Color::Black });
    box.setOutlineThickness(2);

    sf::RectangleShape box2;
    box2.setSize({ 505, 495 });
    box2.setPosition({ 685, 395 });
    box2.setFillColor({ 40, 40, 40, 165 });
    box2.setOutlineColor({ sf::Color::Black });
    box2.setOutlineThickness(2);

    sf::RectangleShape box3;
    box3.setSize({ 665, 380 });
    box3.setPosition({ 10, 510 });
    box3.setFillColor({ 55, 50, 50, 220 });
    box3.setOutlineColor({ sf::Color::Black });
    box3.setOutlineThickness(3);

    window->draw(box);
    window->draw(box2);
    window->draw(box3);

    drawButtons_texts(Text_zones);

    calc_positions(product_tree1.get_root(), 327, 560, 320);
    drawTree(product_tree1.get_root());

    window->draw(menu);
    window->draw(drug_data);
    window->draw(drug_header);
    window->draw(containing_list);
    window->draw(containing_header);
}

void menu1()
{
    std::vector<std::string> Button_labels = { "Add Drug", "Edit Drug", "Replace Product",
                                               "Delete Drug", "Delet category", "Save",
                                               "Exit" };
    std::vector<std::string> text_field_labels = { "Drug label/product...",
                                                   "Drug category/new_product...",
                                                   "Drug id/new id...", "Drug price/new Qte...",
                                                   "File name..." };
    std::vector<std::string> icon_labels = { "add.png", "edit.png", "replace.png", "delete.png",
                                             "delete.png", "save.png", "back.png" };

    initTextZones(text_field_labels, 350, 50);
    initButtons(Button_labels, 250, 50, 1, icon_labels);

    std::string label, category, id, price, filename, text = "";
    int return_code = -1;
    product_tree1.display_categories();
    product_tree1.display_tree_categories_based();
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            // Continious reading zones:
            label = Text_zones[0].getInput();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                int clickedId = handleClick(sf::Mouse::getPosition(*window));
                if (clickedId != -1)
                {
                    // button updated text zones:
                    category = Text_zones[1].getInput();
                    id = Text_zones[2].getInput();
                    price = Text_zones[3].getInput();
                    filename = Text_zones[4].getInput();

                    switch (clickedId)
                    {
                        case 1:  // add drug
                            if (is_number(id) && is_number(price) && !is_number(label) &&
                                !is_number(category))
                            {
                                product_tree1.add_node(stoi(id), label, stof(price), category);
                                for (TextInputField& text : Text_zones)
                                    text.setInput("");
                            }
                            else
                            {
                                if (!is_number(id))
                                    Text_zones[2].setInput("Invalid input.");
                                if (!is_number(price))
                                    Text_zones[3].setInput("Invalid input.");
                                if (is_number(category) || category == "")
                                    Text_zones[1].setInput("Invalid input.");
                                if (is_number(label) || label == "")
                                    Text_zones[0].setInput("Invalid input.");
                            }
                            break;
                        case 2:  // edit
                            if (is_number(id))
                            {
                                if (product_tree1.validate(product_tree1.get_root(), stoi(id)) !=
                                    nullptr)
                                {
                                    return_code = menu2(
                                        product_tree1.validate(product_tree1.get_root(), stoi(id)));
                                    if (return_code != -1)
                                        menu2(product_tree1.validate(product_tree1.get_root(),
                                                                     return_code));
                                    initTextZones(text_field_labels, 350, 50);
                                    initButtons(Button_labels, 250, 50, 1, icon_labels);
                                }
                            }
                            else
                            {
                                Text_zones[2].setInput("Invalid input.");
                            }
                            break;
                        case 3:  // replace product
                            if (is_number(id) && is_number(price) && !is_number(label) &&
                                !is_number(category))
                            {
                                product_tree1.replace_products(product_tree1.get_root(), label,
                                                               category, stoi(id), stoi(price));
                                for (TextInputField& text : Text_zones)
                                    text.setInput("");
                            }
                            else
                            {
                                if (!is_number(id))
                                    Text_zones[2].setInput("Invalid input.");
                                if (!is_number(price))
                                    Text_zones[3].setInput("Invalid input.");
                                if (is_number(category) || category == "")
                                    Text_zones[1].setInput("Invalid input.");
                                if (is_number(label) || label == "")
                                    Text_zones[0].setInput("Invalid input.");
                            }

                            break;
                        case 4:  // delet drug
                            if (is_number(id))
                            {
                                if (product_tree1.validate(product_tree1.get_root(), stoi(id)) !=
                                    nullptr)
                                {
                                    Delet_drug(product_tree1.get_root(), stoi(id));
                                    Text_zones[2].setInput("");
                                }
                            }
                            else
                            {
                                Text_zones[2].setInput("Invalid input.");
                            }
                            break;
                        case 5:  // delet category
                            if (!is_number(category))
                            {
                                Delet_drug_category(product_tree1, product_tree1.get_root(),
                                                    category);
                                Text_zones[1].setInput("");
                            }
                            else
                            {
                                Text_zones[1].setInput("Invalid input.");
                            }
                            break;
                        case 6:  // save
                            if (filename != "")
                            {
                                save_tree(product_tree1.get_root(), filename);
                                Text_zones[4].setInput("");
                            }
                            break;
                        case 7:  // exit
                            window->close();
                            return;
                    }
                    system("cls");
                    product_tree1.display_categories();
                    product_tree1.display_tree_categories_based();
                }
            }

            for (TextInputField& inputField : Text_zones)
                inputField.handleEvent(event, window);
        }

        window->clear(sf::Color::Black);
        Draw_Gui_1(label);
        window->display();
    }
}

void menu0()
{
    std::vector<std::string> Button_labels = { "New Tree", "Load Tree", "Exit" };
    std::vector<std::string> icon_labels = { "add.png", "add.png", "logout.png" };
    std::vector<std::string> text_field_labels = { "File name..." };
    initTextZones(text_field_labels, 200, 50);
    initButtons(Button_labels, 200, 50, 0, icon_labels);  // Initialize buttons for menu0

    sf::Text top_promt, credits_promt;
    std::string Credits = "Credits:\n-Benamireche Abdelhamid\n-Enoch";
    std::string top = "Pharmacy Management Application";
    initializeText(top_promt, { 230, 10 }, 50, { 40, 40, 40 }, top);
    initializeText(credits_promt, { 730, 760 }, 30, sf::Color::White, Credits);

    window = new sf::RenderWindow(sf::VideoMode(1200, 900), "Pharmacy Management");

    std::string file_name;
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                int clickedId = handleClick(sf::Mouse::getPosition(*window));
                if (clickedId != -1)
                {
                    switch (clickedId)
                    {
                        case 1:
                            menu1();
                            return;
                        case 2:
                            file_name = Text_zones[0].getInput();
                            if (file_name == "" || file_name == "file not found.")
                            {
                                Text_zones[0].setInput("file not found.");
                            }
                            else
                            {
                                load_tree(file_name, product_tree1);
                                menu1();
                            }
                            break;
                        case 3:
                            return;
                    }
                }
            }
            for (TextInputField& inputField : Text_zones)
                inputField.handleEvent(event, window);
        }

        window->clear(sf::Color::Black);
        draw_background();
        drawButtons_texts(Text_zones);
        window->draw(top_promt);
        window->draw(credits_promt);
        window->display();
    }
}

int main()
{
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }
    window->setFramerateLimit(60);
    initializeTextures();
    menu0();

    return 0;
}
