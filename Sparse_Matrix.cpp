#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
struct node
{
    int data;
    int no_of_col;
    node *next = NULL;
    node(int val)
    {
        data = val;
        this->next = NULL;
    }
    node(int data, int col)
    {
        this->data = data;
        this->no_of_col = col;
        this->next = NULL;
    }
};
int get_product(node *temp, vector<node *> temp1, int row)
{
    int sum = 0;
    for (int i = 0; i < row; i++)
    {
        node *t = temp1[i]->next;
        node *t1 = temp->next;
        while (t != NULL && t1 != NULL)
        {
            if (t->no_of_col == t1->no_of_col)
            {
                sum += t->data * t1->data;
            }
            else
                sum += 0;
            t = t->next;
            t1 = t1->next;
        }
    }
    return sum;
}
int get_row(ifstream &fin, int &cols)
{
    int i = 0;
    string s;
    int j = 0;
    while (!fin.eof())
    {
        getline(fin, s, '\n');
        if (i == 0)
        {
            for (int k = 0; s[k] != 0; k++)
                if (s[k] == ' ')
                    j++;
        }
        i++;
    }
    cols = j + 1;
    return i;
}
void insert(node *&head, int data, int col)
{
    node *new_node = new node(data, col);

    node *temp = head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = new_node;
}
class sparseMatrix
{
public:
    int M, N;
    vector<node *> rowlist;
    sparseMatrix()
    {
        this->M = 0;
        this->N = 0;
    }
    sparseMatrix(string filename)
    {
        this->read(filename);
    }
    void read(string filename)
    {
        ifstream fin;
        fin.open(filename);
        if (!fin)
        {
            cout << "File not found\n";
            return;
        }
        M = get_row(fin, N);
        fin.close();
        fin.open(filename);
        int data;
        rowlist.reserve(M);
        for (int i = 0; i < M; i++)
            this->rowlist[i] = new node(i + 1);
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                fin >> data;
                if (data != 0)
                {
                    insert(rowlist[i], data, (j + 1));
                }
            }
        }
        fin.close();
    }
    sparseMatrix(const sparseMatrix &obj)
    {
        if (M != 0)
        {
            for (int i = 0; i < M; i++)
            {
                node *temp = this->rowlist[i]->next;
                while (temp != NULL)
                {
                    node *t = temp;
                    temp = temp->next;
                    delete t;
                }
                delete this->rowlist[i];
            }
            this->M = 0;
            this->N = 0;
        }

        this->M = obj.M;
        this->N = obj.N;
        // cout << "M  " << M << "  N  " << N << endl;
        this->rowlist.reserve(M);
        for (int i = 0; i < M; i++)
            this->rowlist[i] = new node(i + 1);
        for (int i = 0; i < M; i++)
        {
            node *temp = obj.rowlist[i]->next;
            while (temp != NULL)
            {
                insert(rowlist[i], temp->data, temp->no_of_col);
                temp = temp->next;
            }
        }
        // cout << "Deep Copy.Copy constructor\n";
    }
    sparseMatrix &operator=(const sparseMatrix &obj)
    {
        if (M != 0)
        {
            for (int i = 0; i < M; i++)
            {
                node *temp = this->rowlist[i]->next;
                while (temp != NULL)
                {
                    node *t = temp;
                    temp = temp->next;
                    delete t;
                }
                delete this->rowlist[i];
            }
            this->M = 0;
            this->N = 0;
        }
        this->M = obj.M;
        this->N = obj.N;
        this->rowlist.reserve(M);
        for (int i = 0; i < M; i++)
            this->rowlist[i] = new node(i + 1);
        for (int i = 0; i < M; i++)
        {
            node *temp = obj.rowlist[i]->next;
            while (temp != NULL)
            {
                insert(rowlist[i], temp->data, temp->no_of_col);
                temp = temp->next;
            }
        }
        // cout << "Deep Copy.Assignment operator\n";
        return *this;
    }
    sparseMatrix operator+(const sparseMatrix &obj)
    {
        if (this->M != obj.M || this->N != obj.N)
        {
            cout << "Addition not Possible\n";
            throw(-1);
        }
        sparseMatrix result;
        result.M = M;
        result.N = N;
        result.rowlist.reserve(M);
        for (int i = 0; i < M; i++)
            result.rowlist[i] = new node(i + 1);
        for (int i = 0; i < M; i++)
        {
            node *temp = rowlist[i]->next;
            node *temp1 = obj.rowlist[i]->next;
            while (temp != NULL && temp1 != NULL)
            {
                if (temp->no_of_col == temp1->no_of_col)
                {
                    insert(result.rowlist[i], (temp->data + temp1->data), temp->no_of_col);
                    temp = temp->next;
                    temp1 = temp1->next;
                }
                else if (temp->no_of_col < temp1->no_of_col)
                {
                    insert(result.rowlist[i], temp->data, temp->no_of_col);
                    temp = temp->next;
                }
                else if (temp->no_of_col > temp1->no_of_col)
                {
                    insert(result.rowlist[i], temp1->data, temp1->no_of_col);
                    temp1 = temp1->next;
                }
            }
            while (temp != NULL)
            {
                insert(result.rowlist[i], temp->data, temp->no_of_col);
                temp = temp->next;
            }
            while (temp1 != NULL)
            {
                insert(result.rowlist[i], temp1->data, temp1->no_of_col);
                temp1 = temp1->next;
            }
        }
        return result;
    }
    ~sparseMatrix()
    {
        for (int i = 0; i < M; i++)
        {
            node *temp = this->rowlist[i]->next;
            while (temp != NULL)
            {
                node *t = temp;
                temp = temp->next;
                delete t;
            }
            delete this->rowlist[i];
        }
        this->M = 0;
        this->N = 0;
        // cout << "~sparseMatrix called\n";
    }
    void display()
    {

        for (int i = 0; i < M; i++)
        {
            node *temp = rowlist[i]->next;
            cout << "Row[" << i << "]"
                 << "==>";
            while (temp != NULL)
            {
                cout << "[ " << temp->no_of_col << " , " << temp->data << "]==>";
                temp = temp->next;
            }
            cout << "NULL";
            cout << endl;
        }
        cout << endl;
    }
    bool isSubMatrix(sparseMatrix const &obj)
    {
        if (obj.M > this->M || obj.N > this->N)
            return false;
        int j = 0;
        int counter = 0;
        int c = 0;
        for (int i = 0; i < M; i++)
        {
            node *temp1 = NULL;
            node *temp = rowlist[i]->next;
            if (j < obj.M)
                temp1 = obj.rowlist[j]->next;

            while (temp != NULL && temp1 != NULL)
            {
                bool flag = false;
                if (temp->data == temp1->data)
                {
                    flag = true;
                    node *temp3 = temp->next;
                    node *temp4 = temp1->next;
                    // cout << temp4->data << endl;
                    while (temp3 != NULL && temp4 != NULL)
                    {
                        if (temp3->data != temp4->data)
                            flag = false;
                        temp3 = temp3->next;
                        temp4 = temp4->next;
                    }
                }
                if (flag == false)
                    if (j > 0)
                        j--;
                if (flag == true)
                {
                    j++;
                    counter++;
                    break;
                }

                temp = temp->next;
                if (counter == obj.M)
                    break;
            }
        }
        if (counter == obj.M)
            return true;
        return false;
    }

    sparseMatrix operator*(sparseMatrix o)
    {

        if (this->N != o.M)
        {
            cout << "multiplication Not possible\n";
            throw(-2);
        }
        sparseMatrix s;
        s.M = this->M;
        s.N = o.N;
        s.rowlist.reserve(M);
        for (int i = 0; i < M; i++)
            s.rowlist[i] = new node(i + 1);
        sparseMatrix obj = o.transpose();
        for (int i = 0; i < M; i++)
        {
            node *n = s.rowlist[i];
            for (int k = 0; k < obj.M; k++)
            {
                node *temp = rowlist[i]->next;
                node *temp1 = obj.rowlist[k]->next;
                int s = 0;
                for (; temp != NULL && temp1 != NULL;)
                {
                    if (temp->no_of_col == temp1->no_of_col)
                    {
                        s = s + (temp->data * temp1->data);
                        temp = temp->next;
                        temp1 = temp1->next;
                    }
                    else if (temp->no_of_col < temp1->no_of_col)
                    {
                        temp = temp->next;
                    }
                    else
                    {
                        temp1 = temp1->next;
                    }
                }
                if (s != 0)
                    insert(n, s, k + 1);
            }
        }
        return s;
    }
    sparseMatrix transpose()
    {
        sparseMatrix s;
        s.M = N;
        s.N = M;
        s.rowlist.reserve(N);
        for (int i = 0; i < N; i++)
        {
            s.rowlist[i] = new node(i + 1);
        }
        for (int i = 0; i < N; i++)
        {
            node *temp = s.rowlist[i];
            for (int j = 0; j < M; j++)
            {
                bool flag = false;
                node *temp1 = rowlist[j]->next;
                while (temp1 != NULL)
                {
                    if (temp1->no_of_col == i + 1)
                    {
                        flag = true;
                        break;
                    }
                    temp1 = temp1->next;
                }

                if (temp1 != NULL && flag == true)
                    insert(temp, temp1->data, j + 1);
            }
        }
        return s;
    }
    bool IsSymmetric()
    {
        sparseMatrix s = this->transpose();
        if (this->M != s.M || this->N != s.N)
            return false;
        for (int i = 0; i < M; i++)
        {
            node *temp = rowlist[i]->next;
            node *temp1 = s.rowlist[i]->next;
            while (temp1 != NULL && temp != NULL)
            {
                if (temp->data != temp1->data)
                    return false;
                temp = temp->next;
                temp1 = temp1->next;
            }
        }
        return true;
    }
    bool IsSkewSymmetric()
    {
        sparseMatrix s = this->transpose();
        if (this->M != s.M || this->N != s.N)
            return false;
        for (int i = 0; i < M; i++)
        {
            node *temp = rowlist[i]->next;
            node *temp1 = s.rowlist[i]->next;
            while (temp1 != NULL && temp != NULL)
            {
                if (temp->data != -1 * (temp1->data))
                    return false;
                temp = temp->next;
                temp1 = temp1->next;
            }
        }
        return true;
    }
    sparseMatrix inverse()
    {
        if (this->M != 2 || this->N != 2)
        {
            cout << "inverse not possible Sorry!!!\n";
            throw(-1);
        }
        if (rowlist[0]->next == NULL || rowlist[1]->next == NULL)
        {
            cout << "|A| is zero.Inverse does not exist\n";
            throw(-1);
        }
        sparseMatrix s;
        s = *this;
        node *temp = s.rowlist[0]->next;
        node *temp1 = s.rowlist[1]->next;
        if (temp1 != NULL || temp != NULL)
        {
            int ab = 0, cd = 0;
            if (temp->no_of_col == 1)
                ab += temp->data;
            if (temp1->no_of_col == 2)
                ab *= temp1->data;
            if (temp1->next != NULL)
            {
                ab *= temp1->next->data;
            }
            else
                ab *= 0;
            if (temp->no_of_col == 2)
                cd += temp->data;
            if (temp->next != NULL)
                cd += temp->next->data;
            if (temp1->no_of_col == 1)
                cd *= temp1->data;
            else
                cd = 0;
            int mod = ab - cd;
            cout << "mod:- " << mod << endl;
            if (mod == 0)
            {
                cout << "|A| is zero so Inverse is not Possible\n";
                return *this;
            }
            int swap1 = 0, swap2 = 0;
            if (temp->no_of_col == 1)
                swap1 = temp->data;
            if (temp1->no_of_col == 2)
            {
                swap2 = temp1->data;
                temp1->data = swap1;
            }
            if (temp1->next != NULL)
                if (temp1->next->no_of_col == 2)
                {
                    swap2 = temp1->next->data;
                    temp1->next->data = swap1;
                }
            if (temp->no_of_col == 1)
                temp->data = swap2;
            s.display();
            if (temp->no_of_col == 2)
                temp->data -= 2 * (temp->data);
            else if (temp->next != NULL)
            {
                if (temp->next->no_of_col == 2)
                    temp->next->data -= 2 * (temp->next->data);
            }
            if (temp1->no_of_col == 1)
                temp1->data -= 2 * (temp1->data);
            s.display();
            for (int i = 0; i < 2; i++)
            {
                node *t = s.rowlist[i]->next;
                while (t != NULL)
                {
                    t->data /= mod;
                    t = t->next;
                }
            }
        }

        return s;
    }
};

int main()
{
    sparseMatrix as("add.txt");
    sparseMatrix as1("add1.txt");
    sparseMatrix asf;
    cout << "Addition is:-\n";
    asf = as + as1;
    asf.display();
    sparseMatrix as2("addt1.txt");
    sparseMatrix as3("add1t1.txt");
    sparseMatrix asr = as2 + as3;
    asr.display();
    // sparseMatrix mul("mult2.txt");
    // sparseMatrix mul1("mul1t2.txt");
    // cout << "Multiplication is:-\n";
    // sparseMatrix mulr;
    // mulr = mul * mul1;
    // mulr.display();
    // sparseMatrix mul2("mult1.txt");
    // sparseMatrix mul3("mul1t1.txt");
    // sparseMatrix sm = mul2 * mul3;
    // sm.display();
    // sparseMatrix mul4("mult2.txt");
    // sparseMatrix mul5("mul1t2.txt");
    // sparseMatrix sm1 = mul2 * mul3;
    // sm1.display();
    sparseMatrix inv("inverse.txt");
    sparseMatrix in = inv.inverse();
    cout << "Inverse is:\n";
    in.display();
    sparseMatrix iss("issub.txt");
    sparseMatrix iss1("issub1.txt");
    iss.display();
    iss1.display();
    if (iss.isSubMatrix(iss1))
    {
        cout << "ISsub matrix\n";
    }
    else
        cout << "not an issub matrix\n";
    sparseMatrix iss2("issubt2.txt");
    sparseMatrix iss3("issub1t2.txt");
    iss2.display();
    iss3.display();
    if (iss2.isSubMatrix(iss3))
    {
        cout << "ISsub matrix\n";
    }
    else
        cout << "not an issub matrix\n";
    sparseMatrix iss4("issubt3.txt");
    sparseMatrix iss5("issub1t3.txt");
    iss4.display();
    iss5.display();
    if (iss4.isSubMatrix(iss5))
    {
        cout << "ISsub matrix\n";
    }
    else
        cout << "not an issub matrix\n";
    sparseMatrix iss6("issubt4.txt");
    sparseMatrix iss7("issub1t4.txt");
    iss6.display();
    iss7.display();
    if (iss6.isSubMatrix(iss7))
    {
        cout << "ISsub matrix\n";
    }
    else
        cout << "not an issub matrix\n";
    sparseMatrix sp("sym3.txt");
    if (sp.IsSymmetric())
        cout << "Symmetric Matrix\n";
    else if (sp.IsSkewSymmetric())
        cout << "skew-Symmetric\n";
    else
        cout << "not symmetric neither skew symmetric\n";

    return 0;
}
