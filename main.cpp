#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<stdexcept>
using namespace std;

class Kant;
class Node;

class Node : public std::enable_shared_from_this<Node>{
private:
    size_t ID;
    vector<shared_ptr<Kant>> KantListe; //holde plass for kanter
public:
    Node(size_t id) : ID(id) {}
    size_t hentID() const {return ID;}
    const vector<shared_ptr<Kant>> hentListe() const {return KantListe;}

    void AddEdge(const shared_ptr<Kant>& kant) {
        KantListe.push_back(kant);
    }

    void FjernKant(const shared_ptr<Kant>& kant) {
        for (auto it = KantListe.begin(); it != KantListe.end(); it++) {
            if (*it == kant) {
                it = KantListe.erase(it);
                return;
            }
        }
    }

    void PrintNaboer() const {
        cout << "Node med ID: " << ID << " har disse naboene: " << endl;
        for (const auto& it : KantListe) {
            auto nabo = it->HentAndreNode(const_cast<Node*>(this)->shared_from_this());
        }
    }


};

class Kant {
private:
    shared_ptr<Node> node1;
    shared_ptr<Node> node2;
    int vekt;

public:
    Kant(const shared_ptr<Node>& Node1, const shared_ptr<Node>& Node2, int V=0) : node1(Node1), node2(Node2), vekt(V) {}


    shared_ptr<Node> HentAndreNode(const shared_ptr<Node>& node) {
        if (node == node1) { return node2;}
        if (node == node2) { return node1;}
        cout << "Denne noden er ikke koblet med noen andre noder" << endl;
        return nullptr;
    }

    //vi sjekker om en node er en del av en lagd kant
    bool HarKant(const shared_ptr<Node>& node) {
        if (node == node1 || node == node2) { return true; }
        return false;
    }

    bool KantEksisterer(const size_t& id1, const size_t& id2) {
        if (node1->hentID() == id1 && node2->hentID() == id2 || node1->hentID() == id2 && node2->hentID() == id1) {
            return true;
        }
        return false;
    }

    //ekstra funksjoner med vekt verdier(mer relevant for del 3)
    int HentVekt() const {return vekt;}
    void BestemVekt(const int V){ vekt = V;}

    void PrintKant() {
        cout << node1->hentID() << "<->" << node2->hentID() << endl;
    }
};


class Graf {
public:
    vector<shared_ptr<Node>> GrafListe;
    vector<shared_ptr<Kant>> AlleKanter; //skal inneholde kantene for hele grafen

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
            for (const auto& n: AlleKanter) {
                if (n->KantEksisterer(id1, id2)){cout << "kant eksisterer!" << endl; return;}
            }

            auto NyKant = make_shared<Kant>(node1, node2);
            AlleKanter.push_back(NyKant);
            node1->AddEdge(NyKant);
            node2->AddEdge(NyKant);
        }
        else {
            cout << "Kunne ikke lage en kant: en eller ingen noder funnet" << endl;
        }
    }

    // del 1 oppgave 3
    void SlettKant(size_t id1, size_t id2) {

        if (!NodeEksisterer(id1) && !NodeEksisterer(id2)) {  return; }
        if (NodeEksisterer(id1) != (NodeEksisterer(id2))) { // we want BOTH of them to exist
            cout << " begge noder må eksistere" << id1 << "og" << id2 << endl;
            return;
        }

        auto node1 = FinnNode(id1);
        auto node2 = FinnNode(id2);

        for (auto kant = AlleKanter.begin(); kant != AlleKanter.end(); ++kant) {
            if((*kant)->KantEksisterer(id1, id2)) {
                node1->FjernKant(*kant);
                node2->FjernKant(*kant);
                kant = AlleKanter.erase(kant);
                return;
            }
        }


    }

    void SlettNode(size_t id) {

        if (!NodeEksisterer(id)) { cout << "Noden eksisterer ikke!"; return; }
        const auto NodeDelete = FinnNode(id);

        //sjekker for kanter og sletter de:
        for(auto kant = AlleKanter.begin(); kant != AlleKanter.end(); ++kant) {

            if ((*kant)->HarKant(NodeDelete)) {
                auto nabo = (*kant)->HentAndreNode(NodeDelete);
                SlettKant(id, nabo->hentID());
            }

        }

        for (auto it = GrafListe.begin(); it != GrafListe.end(); ++it) {
            if (*it == NodeDelete) {
                it = GrafListe.erase(it);
                return; // har slettet noden
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
        node->PrintNaboer();
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