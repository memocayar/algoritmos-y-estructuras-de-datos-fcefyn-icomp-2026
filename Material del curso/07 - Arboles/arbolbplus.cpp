#include <iostream>
#include <vector>
#include <queue> // Exclusivamente para el recorrido BFS en la impresion por niveles

// ============================================================================
// 1. PARAMETROS DEL ARBOL B+ (M = 3)
// ============================================================================
// Orden M = 3 (arbol 2-3):
// - Capacidad maxima: M - 1 = 2 claves por nodo. (3 claves = OVERFLOW -> Split)
// - Capacidad minima: ceil(3 / 2) - 1 = 1 clave por nodo no raiz.
//   (0 claves = UNDERFLOW -> Borrow Left, Borrow Right o Merge)
const int M = 3;
const int MIN_KEYS = (M + 1) / 2 - 1; // MIN_KEYS = 1

// ============================================================================
// 2. ESTRUCTURA DEL NODO: BPlusNode
// ============================================================================
struct BPlusNode {
    bool is_leaf;                     // true: hoja (datos); false: interno (indices)
    std::vector<int> keys;            // Vector de claves
    std::vector<BPlusNode*> children; // Punteros a hijos (solo si is_leaf == false)
    BPlusNode* next;                  // Enlace a la siguiente hoja (solo si is_leaf == true)

    BPlusNode(bool leaf) : is_leaf(leaf), next(nullptr) {}
};

// ============================================================================
// 3. OPERACIONES MANUALES SOBRE ARREGLOS (Sin Iteradores)
// ============================================================================

// Inserta 'val' en 'vec' en la posicion 'pos' desplazando hacia la derecha
void insert_at(std::vector<int>& vec, int pos, int val) {
    vec.push_back(0);
    for (int i = (int)vec.size() - 1; i > pos; i--) {
        vec[i] = vec[i - 1];
    }
    vec[pos] = val;
}

// Inserta un puntero a hijo en 'pos' desplazando hacia la derecha
void insert_child_at(std::vector<BPlusNode*>& vec, int pos, BPlusNode* val) {
    vec.push_back(nullptr);
    for (int i = (int)vec.size() - 1; i > pos; i--) {
        vec[i] = vec[i - 1];
    }
    vec[pos] = val;
}

// Elimina el elemento en 'pos' desplazando hacia la izquierda
void erase_at(std::vector<int>& vec, int pos) {
    for (int i = pos; i < (int)vec.size() - 1; i++) {
        vec[i] = vec[i + 1];
    }
    vec.pop_back();
}

// Elimina el puntero a hijo en 'pos' desplazando hacia la izquierda
void erase_child_at(std::vector<BPlusNode*>& vec, int pos) {
    for (int i = pos; i < (int)vec.size() - 1; i++) {
        vec[i] = vec[i + 1];
    }
    vec.pop_back();
}

// ============================================================================
// 4. CLASE ARBOL B+
// ============================================================================
class BPlusTree {
private:
    BPlusNode* root;

    // Busca el indice del hijo por el cual descender usando un recorrido lineal
    int find_child_index(BPlusNode* parent, int key) {
        int idx = 0;
        while (idx < (int)parent->keys.size() && key >= parent->keys[idx]) {
            idx++;
        }
        return idx;
    }

    // Insercion recursiva bottom-up tradicional
    bool insert_recursive(BPlusNode* curr, int key, int& promoted_key, BPlusNode*& new_child) {
        if (curr->is_leaf) {
            int pos = 0;
            while (pos < (int)curr->keys.size() && curr->keys[pos] < key) {
                pos++;
            }

            if (pos < (int)curr->keys.size() && curr->keys[pos] == key) {
                return false; // Clave duplicada
            }

            insert_at(curr->keys, pos, key);

            // Verificacion de OVERFLOW (keys.size() == 3)
            if ((int)curr->keys.size() >= M) {
                new_child = new BPlusNode(true);
                int mid = (int)curr->keys.size() / 2; // mid = 1

                for (int i = mid; i < (int)curr->keys.size(); i++) {
                    new_child->keys.push_back(curr->keys[i]);
                }
                while ((int)curr->keys.size() > mid) {
                    curr->keys.pop_back();
                }

                new_child->next = curr->next;
                curr->next = new_child;

                promoted_key = new_child->keys[0];
                return true;
            }
            return false;
        }

        int idx = find_child_index(curr, key);
        int up_key;
        BPlusNode* split_node = nullptr;

        bool child_split = insert_recursive(curr->children[idx], key, up_key, split_node);
        if (!child_split) return false;

        insert_at(curr->keys, idx, up_key);
        insert_child_at(curr->children, idx + 1, split_node);

        if ((int)curr->keys.size() >= M) {
            new_child = new BPlusNode(false);
            int mid = (int)curr->keys.size() / 2;

            promoted_key = curr->keys[mid];

            for (int i = mid + 1; i < (int)curr->keys.size(); i++) {
                new_child->keys.push_back(curr->keys[i]);
            }
            for (int i = mid + 1; i < (int)curr->children.size(); i++) {
                new_child->children.push_back(curr->children[i]);
            }

            while ((int)curr->keys.size() > mid) {
                curr->keys.pop_back();
            }
            while ((int)curr->children.size() > mid + 1) {
                curr->children.pop_back();
            }

            return true;
        }

        return false;
    }

    // Corrige el deficit de claves (underflow) cuando un hijo queda con 0 claves
    void fix_underflow(BPlusNode* parent, int idx) {
        BPlusNode* child = parent->children[idx];

        // --------------------------------------------------------------------
        // 1. PRESTAMO DESDE HERMANO IZQUIERDO (Borrow Left)
        // --------------------------------------------------------------------
        if (idx > 0 && (int)parent->children[idx - 1]->keys.size() > MIN_KEYS) {
            BPlusNode* left_sibling = parent->children[idx - 1];

            if (child->is_leaf) {
                int borrowed = left_sibling->keys.back();
                left_sibling->keys.pop_back();

                insert_at(child->keys, 0, borrowed);
                parent->keys[idx - 1] = child->keys[0];
            } else {
                int borrowed_key = left_sibling->keys.back();
                left_sibling->keys.pop_back();
                BPlusNode* borrowed_child = left_sibling->children.back();
                left_sibling->children.pop_back();

                insert_at(child->keys, 0, parent->keys[idx - 1]);
                insert_child_at(child->children, 0, borrowed_child);
                parent->keys[idx - 1] = borrowed_key;
            }
            return;
        }

        // --------------------------------------------------------------------
        // 2. PRESTAMO DESDE HERMANO DERECHO (Borrow Right)
        // Se ejecuta si:
        // - El hermano izquierdo no existe (idx == 0) o no puede prestar.
        // - El hermano derecho existe y tiene > MIN_KEYS (2 claves para M=3).
        // --------------------------------------------------------------------
        if (idx < (int)parent->children.size() - 1 && (int)parent->children[idx + 1]->keys.size() > MIN_KEYS) {
            BPlusNode* right_sibling = parent->children[idx + 1];

            if (child->is_leaf) {
                // Se toma la primera clave del hermano derecho
                int borrowed = right_sibling->keys[0];
                erase_at(right_sibling->keys, 0);

                // Se anexa al final del nodo actual que sufrio underflow
                child->keys.push_back(borrowed);

                // La nueva primera clave del hermano derecho actualiza el separador en el padre
                parent->keys[idx] = right_sibling->keys[0];
            } else {
                // Rotacion en nodos internos a traves de la clave separadora del padre
                int borrowed_key = right_sibling->keys[0];
                erase_at(right_sibling->keys, 0);

                BPlusNode* borrowed_child = right_sibling->children[0];
                erase_child_at(right_sibling->children, 0);

                // La clave del padre baja a este nodo y el hijo transferido se asocia
                child->keys.push_back(parent->keys[idx]);
                child->children.push_back(borrowed_child);

                // La clave del hermano derecho asciende al padre
                parent->keys[idx] = borrowed_key;
            }
            return;
        }

        // --------------------------------------------------------------------
        // 3. FUSION OBLIGATORIA (Merge)
        // --------------------------------------------------------------------
        if (idx > 0) {
            merge_children(parent, idx - 1);
        } else {
            merge_children(parent, idx);
        }
    }

    // Fusiona el hijo left_idx con el hijo left_idx + 1
    void merge_children(BPlusNode* parent, int left_idx) {
        BPlusNode* left = parent->children[left_idx];
        BPlusNode* right = parent->children[left_idx + 1];

        if (left->is_leaf) {
            for (int i = 0; i < (int)right->keys.size(); i++) {
                left->keys.push_back(right->keys[i]);
            }
            left->next = right->next;
        } else {
            left->keys.push_back(parent->keys[left_idx]);
            for (int i = 0; i < (int)right->keys.size(); i++) {
                left->keys.push_back(right->keys[i]);
            }
            for (int i = 0; i < (int)right->children.size(); i++) {
                left->children.push_back(right->children[i]);
            }
        }

        erase_at(parent->keys, left_idx);
        erase_child_at(parent->children, left_idx + 1);

        delete right;
    }

    // Eliminacion recursiva tradicional con busqueda manual
    bool remove_recursive(BPlusNode* curr, int key) {
        if (curr->is_leaf) {
            int target_idx = -1;
            for (int i = 0; i < (int)curr->keys.size(); i++) {
                if (curr->keys[i] == key) {
                    target_idx = i;
                    break;
                }
            }

            if (target_idx == -1) return false;

            erase_at(curr->keys, target_idx);
            return true;
        }

        int idx = find_child_index(curr, key);
        bool removed = remove_recursive(curr->children[idx], key);
        if (!removed) return false;

        if ((int)curr->children[idx]->keys.size() < MIN_KEYS) {
            fix_underflow(curr, idx);
        }

        return true;
    }

public:
    BPlusTree() : root(new BPlusNode(true)) {}

    void insert(int key) {
        int promoted_key;
        BPlusNode* split_child = nullptr;
        bool root_split = insert_recursive(root, key, promoted_key, split_child);

        if (root_split) {
            BPlusNode* new_root = new BPlusNode(false);
            new_root->keys.push_back(promoted_key);
            new_root->children.push_back(root);
            new_root->children.push_back(split_child);
            root = new_root;
        }
    }

    void remove(int key) {
        remove_recursive(root, key);

        if (!root->is_leaf && root->keys.empty()) {
            BPlusNode* old_root = root;
            root = root->children[0];
            delete old_root;
        }
    }

    int get_depth() const {
        int depth = 0;
        BPlusNode* curr = root;
        while (curr != nullptr) {
            depth++;
            if (curr->is_leaf) break;
            curr = curr->children[0];
        }
        return depth;
    }

    void print_tree_levels() const {
        std::cout << "\n--- ESTRUCTURA POR NIVELES (Profundidad: " << get_depth() << ") ---\n";
        std::queue<BPlusNode*> q;
        q.push(root);

        int level = 1;
        while (!q.empty()) {
            int level_size = (int)q.size();
            std::cout << "Nivel " << level << ": ";

            for (int i = 0; i < level_size; i++) {
                BPlusNode* node = q.front();
                q.pop();

                std::cout << "[";
                for (int k = 0; k < (int)node->keys.size(); k++) {
                    std::cout << node->keys[k] << (k + 1 < (int)node->keys.size() ? "," : "");
                }
                std::cout << "] ";

                if (!node->is_leaf) {
                    for (int c = 0; c < (int)node->children.size(); c++) {
                        q.push(node->children[c]);
                    }
                }
            }
            std::cout << "\n";
            level++;
        }
        std::cout << "---------------------------------------------------------\n";
    }

    void print_leaves() const {
        BPlusNode* curr = root;
        while (!curr->is_leaf) {
            curr = curr->children[0];
        }

        std::cout << "Lista de hojas: ";
        while (curr != nullptr) {
            std::cout << "[";
            for (int i = 0; i < (int)curr->keys.size(); i++) {
                std::cout << curr->keys[i] << (i + 1 < (int)curr->keys.size() ? "," : "");
            }
            std::cout << "] -> ";
            curr = curr->next;
        }
        std::cout << "NULL\n";
    }
};

// ============================================================================
// 5. MAIN: PROFUNDIDAD 4 Y PRUEBAS COMPLETAS (INCLUYENDO BORROW RIGHT)
// ============================================================================
int main() {
    BPlusTree tree;

    std::cout << "========================================================\n";
    std::cout << " FASE 1: CONSTRUCCION DEL ARBOL HASTA PROFUNDIDAD 4\n";
    std::cout << "========================================================\n";
    // Para M = 3, insertamos 22 claves (10, 20, ..., 220) para desbordar
    // la capacidad maxima de 3 niveles (18 claves) y generar la raiz en Nivel 4.
    for (int i = 1; i <= 22; i++) {
        tree.insert(i * 10);
    }

    tree.print_tree_levels();
    tree.print_leaves();

    std::cout << "\n========================================================\n";
    std::cout << " CASO 1: ELIMINACION SIMPLE (Sin Underflow)\n";
    std::cout << "========================================================\n";
    // La ultima hoja contiene {210, 220}. Al borrar 220, retiene 1 clave (>= MIN_KEYS).
    std::cout << "-> Eliminando 220...\n";
    tree.remove(220);
    tree.print_tree_levels();
    tree.print_leaves();

    std::cout << "\n========================================================\n";
    std::cout << " CASO 2: ELIMINACION CON PRESTAMO IZQUIERDO (Borrow Left)\n";
    std::cout << "========================================================\n";
    // La ultima hoja tiene {210}. Al borrar 210, queda con 0 claves.
    // Su hermano izquierdo {190, 200} tiene 2 claves (> MIN_KEYS); presta el 200.
    std::cout << "-> Eliminando 210 (underflow resuelto con prestamo izquierdo)...\n";
    tree.remove(210);
    tree.print_tree_levels();
    tree.print_leaves();

    std::cout << "\n========================================================\n";
    std::cout << " CASO 3: ELIMINACION CON PRESTAMO DERECHO (Borrow Right)\n";
    std::cout << "========================================================\n";
    // Estado en el extremo izquierdo:
    // - Primera hoja (idx = 0): [10] (1 clave).
    // - Hermano izquierdo: NO TIENE (idx == 0).
    // - Hermano derecho: [20, 30] (2 claves > MIN_KEYS).
    // Al borrar 10, la primera hoja cae a 0 claves. No hay hermano izquierdo,
    // pero el derecho tiene sobrante -> Toma prestado el 20 del hermano derecho.
    // El separador en el padre se actualiza con la nueva primera clave del derecho (30).
    std::cout << "-> Eliminando 10 (la hoja queda vacia y toma prestado del hermano DERECHO)...\n";
    tree.remove(10);
    tree.print_tree_levels();
    tree.print_leaves();

    std::cout << "\n========================================================\n";
    std::cout << " CASO 4: ELIMINACION CON FUSION (Merge)\n";
    std::cout << "========================================================\n";
    // Ahora la primera hoja tiene [20] y la segunda tiene [30] (ambas con 1 clave).
    // Al borrar 20, la primera hoja queda vacia y su hermano derecho no puede prestar.
    // Se produce una fusion (Merge): [30] absorbe la posicion y el padre pierde un separador.
    std::cout << "-> Eliminando 20 (underflow que fuerza fusion con el hermano derecho)...\n";
    tree.remove(20);
    tree.print_tree_levels();
    tree.print_leaves();

    std::cout << "\n========================================================\n";
    std::cout << " CASO 5: FUSION EN CASCADA Y REDUCCION DE PROFUNDIDAD (4 -> 3)\n";
    std::cout << "========================================================\n";
    // Borramos en bloque claves contiguas del subarbol derecho para vaciar
    // hojas y forzar fusiones en los niveles internos 3 y 2, contrayendo la raiz.
    std::cout << "-> Eliminando en cascada: 200, 190, 180, 170, 160...\n";
    int a_borrar[] = {200, 190, 180, 170, 160};
    for (int k : a_borrar) {
        tree.remove(k);
    }

    tree.print_tree_levels();
    tree.print_leaves();

    std::cout << "\nProfundidad final tras la cascada: " << tree.get_depth() << " niveles.\n";

    return 0;
}