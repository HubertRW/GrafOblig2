#include<iostream>
#include<string>
#include<vector>
#include<memory>

using namespace std;

class Node {
private:
    size_t ID;

public:
    vector<shared_ptr<Node>> NodeListe; //holde plass for naboene; kanter
    Node(size_t id) : ID(id) {}
    size_t hentID() const {return ID;}

    void PrintNabo() const {
        cout << "Node med ID: " << ID << " har disse naboene: " << endl;
        for (const auto& it : NodeListe) {
            cout << it->hentID() << endl;
        }
    }

    void FjernNabo(size_t id) {
        for (auto it = NodeListe.begin(); it != NodeListe.end(); ++it) {
            if ((*it)->hentID() == id) {
                it = NodeListe.erase(it);
                return;
            }
        }
    }

};

class Graf {
public:
    vector<shared_ptr<Node>> GrafListe;

    bool NodeEksisterer(const size_t id) {
        for (const auto& it : GrafListe) {
            if (it->hentID() == id) {
                //cout << "ID eksisterer allerede" << endl;
                return true;
            }
        }
        return false;
    }

    shared_ptr<Node> FinnNode(size_t id) {
            for (auto node: GrafListe) {
                if (node->hentID() == id) {
                    return node;
                }
            }
            return nullptr;
        }

    // del 1 oppgave 2
    void LeggTilNode(const size_t id) {
        if (NodeEksisterer(id)) { return;}
        GrafListe.push_back(make_shared<Node>(id));
    }

    void LeggTilKant(size_t id1, size_t id2) {
        shared_ptr<Node> node1 = FinnNode(id1);
        shared_ptr<Node> node2 = FinnNode(id2);

        if (FinnNode(id1) && FinnNode(id2)) {
            for (const auto &nabo: node1->NodeListe) {
                if (nabo->hentID() == id2) {
                    cout << "En kant finnes allerede mellom " << id1 << "og" << id2 << endl;
                    return;
                }
            }

            // adjacency list: begge retninger
            node1->NodeListe.push_back(node2);
            node2->NodeListe.push_back(node1);
        }
        else {
            cout << "Kunne ikke lage en kant: en eller ingen noder funnet" << endl;
        }
    }

    // del 1 oppgave 3
    void SlettKant(size_t id1, size_t id2) {

        if (!NodeEksisterer(id1) && !NodeEksisterer(id2)) { cout << "test"; return; }
        if (NodeEksisterer(id1) != (NodeEksisterer(id2))) { // we want BOTH of them to exist
            cout << " begge noder må eksistere" << id1 << "og" << id2 << endl;
            return;
        }

        shared_ptr<Node> node1 = FinnNode(id1);
        shared_ptr<Node> node2 = FinnNode(id2);

        node1->FjernNabo(id2);
        node2->FjernNabo(id1);


    }

    void SlettNode(size_t id) {

        if (!NodeEksisterer(id)) { return; }
        const auto NodeDelete = FinnNode(id);

        for ( auto it = GrafListe.begin(); it != GrafListe.end(); ++it) {
            if ((*it)->hentID() == NodeDelete->hentID()) { //located the node
                //fjern kanter først
                for (const auto& nabo : NodeDelete->NodeListe) {
                    nabo->FjernNabo(id);
                }
                it = GrafListe.erase(it);
                return;
            }

        }
    }

};

void PrintGraphList(const Graf& graf) {
    for (const auto& node : graf.GrafListe) {
        cout << node->hentID() << endl;
    }
}

void PrintAdjacencyList(const Graf& graf) {
    for (const auto& node : graf.GrafListe) {
        node->PrintNabo();
    }
}

void Test1() {
    Graf G;
    G.LeggTilNode(13);
    G.LeggTilNode(21);
    G.LeggTilNode(15);
    G.LeggTilNode(20);
    G.LeggTilNode(7);
    G.LeggTilNode(8);

    cout << "Første liste:" << endl;
    PrintGraphList(G);

    cout << "Etter forandring:" << endl;
    G.SlettNode(21);
    G.SlettNode(15);
    G.LeggTilNode(18);
    G.LeggTilNode(4);
    PrintGraphList(G);
}

void Test2Del1() {
    Graf G;
    G.LeggTilNode(1);
    G.LeggTilNode(5);
    G.LeggTilNode(6);
    G.LeggTilNode(4);
    G.LeggTilNode(3);
    G.LeggTilNode(10);
    G.LeggTilNode(9);

    G.LeggTilKant(3,5);
    G.LeggTilKant(4,3);
    G.LeggTilKant(10,1);

    cout << "Nabo liste:" << endl;
    PrintAdjacencyList(G);

    cout << "Etter endringer:" << endl;
    G.SlettKant(3,5);
    PrintAdjacencyList(G);
}

int main() {

    Test1();

}