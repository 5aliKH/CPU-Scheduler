#include <iostream>    // مكتبة الإدخال والإخراج الأساسية
#include <string>      // للتعامل مع النصوص والأسماء
#include <iomanip>     // لتنسيق شكل الجداول والمسافات
#include <cstdlib>     // للعمليات العامة مثل مسح الشاشة
#include <windows.h>   // لتلوين النصوص في شاشة الكونسول

using namespace std;

// دالة لتغيير لون النص المطبوع في الكونسول
void setTextColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

// هيكل العملية ببياناتها الرياضية والبرمجية
struct Process {
    string id;        // اسم العملية
    int at, bt, pr;   // وقت الوصول، وقت التنفيذ، والأولوية
    int rem_bt;       // الوقت المتبقي (للخوارزميات القاطعة)
    int st, ct;       // وقت البداية ووقت النهاية
    int wt, tat;      // وقت الانتظار ووقت الإنجاز
    bool finished;    // علامة لتحديد انتهاء العملية

    Process() {}      // منشئ افتراضي
    Process(string i, int a, int b, int p) {
        id = i; at = a; bt = b; pr = p;
        rem_bt = b; finished = false; 
        st=0; ct=0; wt=0; tat=0;
    }
};

// عقدة لربط العمليات في القائمة المرتبطة يدوياً
struct Node {
    Process data;
    Node* next;
    Node(Process p) { data = p; next = NULL; } 
};

// كلاس لإدارة العمليات المخزنة في النظام
class ProcessStore {
public:
    Node* head;
    ProcessStore() { head = NULL; } 

    // إضافة عملية جديدة في نهاية القائمة
    void append(Process p) {
        Node* newNode = new Node(p);
        if(!head) head = newNode;
        else {
            Node* temp = head;
            while(temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

    // عمل نسخة من القائمة لاستخدامها في الحسابات
    ProcessStore copy() {
        ProcessStore newList;
        Node* temp = head;
        while(temp) {
            newList.append(temp->data);
            temp = temp->next;
        }
        return newList;
    }

    // ترتيب العمليات حسب وقت الوصول لضمان دقة المحاكاة
    void sortByArrival() {
        if(!head || !head->next) return;
        bool swapped;
        do {
            swapped = false;
            Node* current = head;
            while(current->next) {
                if(current->data.at > current->next->data.at) {
                    Process temp = current->data; current->data = current->next->data; current->next->data = temp;
                    swapped = true;
                }
                current = current->next;
            }
        } while(swapped);
    }
    
    // إخلاء الذاكرة ومسح جميع العمليات
    void reset() {
         while(head) { Node* temp = head; head = head->next; delete temp; }
    }
    
    // عدد العمليات المسجلة حالياً
    int length() {
        int count = 0; Node* temp = head; while(temp){count++; temp = temp->next;} return count;
    }
};

// كلاس لإدارة طوابير الجدولة (Queue & Priority Queue Logic)
class SchedulerQueue {
public:
    Node* head;
    int mode; // نوع الترتيب: 0 للأقصر، 1 للأهم، 2 للمتبقي

    SchedulerQueue(int m) { head = NULL; mode = m; }

    // تحديد من له الأفضلية في الطابور
    bool compare(Process a, Process b) {
        if(mode==0) return (a.bt == b.bt) ? (a.at < b.at) : (a.bt < b.bt);
        if(mode==1) return (a.pr == b.pr) ? (a.at < b.at) : (a.pr < b.pr);
        if(mode==2) return (a.rem_bt == b.rem_bt) ? (a.at < b.at) : (a.rem_bt < b.rem_bt);
        return false;
    }

    // إضافة عملية للطابور بشكل مرتب (Priority Push)
    void pushOrdered(Process p) {
        Node* n = new Node(p);
        if(!head || compare(p, head->data)) { n->next = head; head = n; }
        else {
            Node* c = head;
            while(c->next && !compare(p, c->next->data)) c = c->next;
            n->next = c->next; c->next = n;
        }
    }
    
    // إضافة للنهاية (لخوارزمية Round Robin)
    void pushEnd(Process p) {
        Node* n = new Node(p);
        if(!head) head = n;
        else {
            Node* t = head;
            while(t->next) t = t->next;
            t->next = n;
        }
    }

    // سحب العملية من مقدمة الطابور (Pop)
    Process pop() {
        if(!head) return Process("",0,0,0);
        Node* t = head; Process p = t->data;
        head = head->next; delete t; return p;
    }
    
    bool isEmpty() { return head == NULL; }
};

// كلاس المكدس لحفظ وعرض سجل النتائج النهائية
class HistoryLogs {
    Node* top;
public:
    HistoryLogs() { top = NULL; }
    void push(Process p) {
        Node* n = new Node(p);
        n->next = top; top = n; // الإضافة في القمة للعرض
    }
    
    // طباعة جدول النتائج النهائي
    void printTable(string title) {
        double wSum = 0, tSum = 0;
        int n = 0;
        Node* t = top;
        
        cout << "\n  "; setTextColor(14); cout << "[ REPORT: " << title << " ]\n"; setTextColor(15);
        cout << "  +------+------+-------+------+------+------+------+\n";
        cout << "  | ID   | ARRV | BURST | PRIO | WAIT | TAT  | END  |\n";
        cout << "  +------+------+-------+------+------+------+------+\n";
        
        while(t) {
            Process p = t->data;
            cout << "  | " << left << setw(4) << p.id 
                 << " | " << setw(4) << p.at << " | " << setw(5) << p.bt 
                 << " | " << setw(4) << p.pr << " | " << setw(4) << p.wt 
                 << " | " << setw(4) << p.tat << " | " << setw(4) << p.ct << " |\n";
            wSum += p.wt; tSum += p.tat;
            n++; t = t->next;
        }
        cout << "  +------+------+-------+------+------+------+------+\n";
        if(n > 0) {
            setTextColor(10); 
            cout << "  >> Avg Waiting Time    : " << (wSum/n) << " ms\n";
            cout << "  >> Avg Turnaround Time : " << (tSum/n) << " ms\n";
        }
        setTextColor(15);
    }
};

// كلاس محرك الجدولة الرئيسي
class CentralScheduler {
public:
    ProcessStore registry;

    void addProcess(string i, int a, int b, int p) {
        registry.append(Process(i,a,b,p));
    }

    // 1. خوارزمية FCFS
    void fcfs() {
        ProcessStore work = registry.copy(); work.sortByArrival(); 
        HistoryLogs logs; int timer = 0;
        setTextColor(11); cout << "\n  [ GANTT ]: "; setTextColor(15);
        Node* curr = work.head;
        while(curr) {
            Process p = curr->data;
            if(timer < p.at) timer = p.at;
            timer += p.bt; p.ct = timer; p.tat = p.ct-p.at; p.wt = p.tat-p.bt;
            cout << "=> " << p.id << " "; logs.push(p); curr = curr->next;
        }
        cout << "\n"; logs.printTable("FCFS");
    }

    // 2. خوارزمية SJF
    void sjf() {
        ProcessStore work = registry.copy(); work.sortByArrival();
        SchedulerQueue pq(0); HistoryLogs logs;
        int timer = 0, done = 0, total = work.length();
        Node* ptr = work.head;
        setTextColor(11); cout << "\n  [ GANTT ]: "; setTextColor(15);
        while(done < total) {
            while(ptr && ptr->data.at <= timer) { pq.pushOrdered(ptr->data); ptr=ptr->next; }
            if(!pq.isEmpty()) {
                Process p = pq.pop();
                timer += p.bt; p.ct = timer; p.tat = p.ct-p.at; p.wt = p.tat-p.bt;
                cout << "=> " << p.id << " "; logs.push(p); done++;
            } else { if(ptr) timer = ptr->data.at; else timer++; }
        }
        cout << "\n"; logs.printTable("SJF");
    }

    // 3. خوارزمية Priority
    void priority() {
        ProcessStore work = registry.copy(); work.sortByArrival();
        SchedulerQueue pq(1); HistoryLogs logs;
        int timer = 0, done = 0, total = work.length();
        Node* ptr = work.head;
        setTextColor(11); cout << "\n  [ GANTT ]: "; setTextColor(15);
        while(done < total) {
            while(ptr && ptr->data.at <= timer) { pq.pushOrdered(ptr->data); ptr=ptr->next; }
            if(!pq.isEmpty()) {
                Process p = pq.pop();
                timer += p.bt; p.ct = timer; p.tat = p.ct-p.at; p.wt = p.tat-p.bt;
                cout << "=> " << p.id << " "; logs.push(p); done++;
            } else { if(ptr) timer = ptr->data.at; else timer++; }
        }
        cout << "\n"; logs.printTable("Priority");
    }

    // 4. خوارزمية Round Robin
    void rr(int q) {
        ProcessStore work = registry.copy(); work.sortByArrival();
        SchedulerQueue rq(0); HistoryLogs logs; 
        int timer = 0, done = 0, total = work.length();
        Node* ptr = work.head;
        setTextColor(11); cout << "\n  [ GANTT ]: "; setTextColor(15);
        if(ptr && ptr->data.at > timer) timer = ptr->data.at;
        while(ptr && ptr->data.at <= timer) { rq.pushEnd(ptr->data); ptr=ptr->next; }
        while(!rq.isEmpty()) {
            Process p = rq.pop();
            int slice = (p.rem_bt < q) ? p.rem_bt : q;
            p.rem_bt -= slice; timer += slice;
            cout << "=> " << p.id << " ";
            while(ptr && ptr->data.at <= timer) { rq.pushEnd(ptr->data); ptr=ptr->next; }
            if(p.rem_bt > 0) rq.pushEnd(p);
            else { p.ct = timer; p.tat = p.ct-p.at; p.wt = p.tat-p.bt; logs.push(p); done++; }
            if(rq.isEmpty() && done < total && ptr) {
                if(timer < ptr->data.at) timer = ptr->data.at;
                while(ptr && ptr->data.at <= timer) { rq.pushEnd(ptr->data); ptr=ptr->next; }
            }
        }
        cout << "\n"; logs.printTable("Round Robin");
    }

    // 5. خوارزمية SRTF
    void srtf() {
        ProcessStore work = registry.copy(); work.sortByArrival();
        SchedulerQueue pq(2); HistoryLogs logs;
        int total = work.length(), done = 0, timer = 0;
        Node* incoming = work.head;
        setTextColor(11); cout << "\n  [ GANTT ]: START "; setTextColor(15);
        if(incoming) timer = incoming->data.at;
        while(done < total) {
            while(incoming && incoming->data.at <= timer) { pq.pushOrdered(incoming->data); incoming = incoming->next; }
            if(!pq.isEmpty()) {
                Process p = pq.pop();
                cout << p.id << "."; timer++; p.rem_bt--; 
                if(p.rem_bt == 0) { p.ct = timer; p.tat = p.ct-p.at; p.wt = p.tat-p.bt; logs.push(p); done++; }
                else pq.pushOrdered(p);
            } else { cout << "_"; timer++; }
        }
        cout << " END\n"; logs.printTable("SRTF");
    }

    // 6. خوارزمية HRRN
    void hrrn() {
        ProcessStore work = registry.copy(); work.sortByArrival();
        HistoryLogs logs; int timer = 0, done = 0, total = work.length();
        setTextColor(11); cout << "\n  [ GANTT ]: "; setTextColor(15);
        while(done < total) {
            Node* best = NULL; double maxRatio = -1.0;
            Node* cur = work.head;
            while(cur) {
                if(!cur->data.finished && cur->data.at <= timer) {
                    double ratio = (double)((timer - cur->data.at) + cur->data.bt) / cur->data.bt;
                    if(ratio > maxRatio) { maxRatio = ratio; best = cur; }
                }
                cur = cur->next;
            }
            if(best != NULL) {
                Process* p = &best->data;
                timer += p->bt; p->ct = timer; p->tat = p->ct - p->at; p->wt = p->tat - p->bt; p->finished = true;
                cout << "=> " << p->id << " "; logs.push(*p); done++;
            } else {
                int nextVal = 99999; cur = work.head;
                while(cur) { if(!cur->data.finished && cur->data.at > timer) if(cur->data.at < nextVal) nextVal = cur->data.at; cur = cur->next; }
                if(nextVal != 99999) timer = nextVal; else timer++;
            }
        }
        cout << "\n"; logs.printTable("HRRN");
    }
};

// دالة عرض الشعار والقائمة
void showMenu() {
    system("cls");
    setTextColor(13);
    cout << "   .d8888b.  888  888 8888888b.   .d88888b.  888b    888  .d88888b.   .d8888b.  \n";
    cout << "  d88P  Y88b 888  888 888   Y88b d88P\" \"Y88b 8888b   888 d88P\" \"Y88b d88P  Y88b \n";
    cout << "  888    888 888  888 888    888 888     888 88888b  888 888     888 Y88b.      \n";
    cout << "  888        88888888 888   d88P 888     888 888Y88b 888 888     888  \"Y888b.   \n";
    cout << "  888        888  888 8888888P\"  888     888 888 Y88b888 888     888     \"Y88b. \n";
    cout << "  888    888 888  888 888 T88b   888     888 888  Y88888 888     888       \"888 \n";
    cout << "  Y88b  d88P 888  888 888  T88b  Y88b. .d88P 888   Y8888 Y88b. .d88P Y88b  d88P \n";
    cout << "   \"Y8888P\"  888  888 888   T88b  \"Y88888P\"  888    Y888  \"Y88888P\"   \"Y8888P\"  \n";
    setTextColor(11);
    cout << "     >> CHRONOS SCHEDULER: CORE SIMULATOR <<\n";
    cout << "     =======================================\n\n";
    setTextColor(15);
}

int main() {
    system("chcp 65001 > nul");
    CentralScheduler engine;
    
    // إضافة بيانات تجريبية (Demo) لضمان العمل السريع
    engine.addProcess("P1", 0, 8, 3); engine.addProcess("P2", 1, 4, 1);
    engine.addProcess("P3", 2, 9, 2);
    
    int choice;
    while(true) {
        showMenu();
        
        // تلميحات للمستخدم وحالة الذاكرة
        setTextColor(8); 
        cout << "  * Hint: Add logic or processes [1] before running [2-7].\n";
        cout << "  * Status: (" << engine.registry.length() << ") active processes in core memory.\n\n";
        
        setTextColor(14); cout << "  [ MAIN DASHBOARD ]\n"; setTextColor(15);
        
        // خيارات القائمة ملونة ومرتبة عمودياً
        setTextColor(10); cout << "  [1] Register New Process    \n";
        setTextColor(11); cout << "  [2] Run Algorithm: FCFS     \n";
        setTextColor(11); cout << "  [3] Run Algorithm: SJF      \n";
        setTextColor(11); cout << "  [4] Run Algorithm: Priority \n";
        setTextColor(11); cout << "  [5] Run Algorithm: RR       \n";
        setTextColor(11); cout << "  [6] Run Algorithm: SRTF     \n";
        setTextColor(11); cout << "  [7] Run Algorithm: HRRN     \n";
        setTextColor(12); cout << "  [9] Wipe Core Memory       \n";
        setTextColor(12); cout << "  [0] Terminate Shutdown     \n";
        
        cout << "\n  SELECT COMMAND > ";
        
        if(!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        if(choice == 0) break; 
        
        switch(choice) {
            case 1: {
                string nm; int a, b, p;
                cout << "\n  ID: "; cin >> nm; cout << "  Arrival: "; cin >> a;
                cout << "  Burst: "; cin >> b; cout << "  Priority: "; cin >> p;
                engine.addProcess(nm, a, b, p); break;
            }
            case 2: engine.fcfs(); system("pause"); break;
            case 3: engine.sjf(); system("pause"); break;
            case 4: engine.priority(); system("pause"); break;
            case 5: {
                int q; cout << "\n  Enter Time Quantum: "; cin >> q;
                engine.rr(q); system("pause"); break;
            }
            case 6: engine.srtf(); system("pause"); break;
            case 7: engine.hrrn(); system("pause"); break;
            case 9: engine.registry.reset(); break;
        }
    }
    return 0;
}
