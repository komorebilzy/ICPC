#include<iostream>
#include<vector>
#include<set>
#include<algorithm>
#include<unordered_map>

using namespace std;
struct Teaminfo {
    string team_name;
    int rank = 0;
    int solvednumber = 0;
    int punishtimesum = 0;
    int timelist[30] = {0};//从小到大排列.记录每一道题的最大通过时间
    int aclast[30] = {0};//1到26是题号，数组值是时间
    int walast[30] = {0};
    int relast[30] = {0};
    int tlelast[30] = {0};
    int submitlast[30] = {0};//最后一次提交的时间
    int submitnumber1[30] = {0};
    int falsenumber[30] = {0};
    int beforefirstac[30] = {0};
    int flagac[30] = {0};//标记第一次ac
    int solvednumber1[30] = {0};//封榜之后的解决问题
    int punishtime[30] = {0};
    int timelist1[30] = {0};
    int flagfrozen[30] = {0};
    int proac;
    int prowa;
    int prore;
    int protle;
    int proall;
    string flagstatus[30];
};
Teaminfo teaminfo;
unordered_map<string, Teaminfo> Information;
vector<string> viewrank;
string order, team_name;

int problemcount;

struct Compare {
    bool operator()(const string &teamname1, const string &teamname2) const {
        const Teaminfo &team1 = Information[teamname1];
        const Teaminfo &team2 = Information[teamname2];
        if (team1.solvednumber != team2.solvednumber) return team1.solvednumber > team2.solvednumber;
        if (team1.punishtimesum != team2.punishtimesum) return team1.punishtimesum < team2.punishtimesum;
        for (int i = problemcount; i >= 1; --i) {
            if (team1.timelist[i] != team2.timelist[i]) return team1.timelist[i] < team2.timelist[i];
        }
        return teamname1 < teamname2;
    }
};

set<string, Compare> realrank;
bool flag = true;
bool flagfreeze = true;
int duration_time;//总时长

int main() {
//    freopen("2.in", "r", stdin);
//    freopen("text.out", "w", stdout);
    while (1) {
        cin >> order;
        if (order == "ADDTEAM") {
            cin >> team_name;
            if (flag) {
                if (realrank.count(team_name) == 0) {
                    teaminfo.team_name = team_name;
                    Information.insert({team_name, teaminfo});

                    realrank.insert(team_name);
                    cout << "[Info]Add successfully.\n";
                } else cout << "[Error]Add failed: duplicated team name.\n";
            } else cout << "[Error]Add failed: competition has started.\n";
        }

        if (order == "START") {
            if (flag) {
                string temp1, temp2;
                cin >> temp1 >> duration_time >> temp2 >> problemcount;
                flag = false;
                cout << "[Info]Competition starts.\n";
                int i = 0;
                for (auto iter = realrank.begin(); iter != realrank.end(); ++iter) {
                    i++;
                    Information[*iter].rank = i;
                }
            } else cout << "[Error]Start failed: competition has started.\n";
        }

        if (order == "SUBMIT") {
            string temp1, temp2, temp3, submit_status;//用函数cin.ignore (字符个数，终点字符)会更简洁
            int time;
            char problem_name;
            cin >> problem_name >> temp1 >> team_name >> temp2 >> submit_status >> temp3 >> time;
            int problemname = problem_name - 'A' + 1;
            Teaminfo &team = Information[team_name];
            team.submitlast[problemname] = time;
            team.proall = problemname;
            team.flagstatus[problemname]=submit_status;
            realrank.erase(team_name);
            if (flagfreeze) {
                if (submit_status == "Accepted") {
                    team.aclast[problemname] = time;
                    team.proac = problemname;
                    if (team.flagac[problemname] == 0) {
                        team.solvednumber++;
                        team.timelist[1] = time;
                        sort(team.timelist,team.timelist+problemcount);
                        team.punishtimesum +=
                                (20 * team.beforefirstac[problemname] + time);
                        team.flagac[problemname] = 1;
                    }

                } else {
                    if (team.flagac[problemname] == 0) {
                        team.falsenumber[problemname]++;
                        team.beforefirstac[problemname]++;
                    }
                    if (submit_status == "Wrong_Answer") {
                        team.walast[problemname] = time;
                        team.prowa = problemname;
                    }
                    if (submit_status == "Runtime_Error") {
                        team.relast[problemname] = time;
                        team.prore = problemname;
                    }
                    if (submit_status == "Time_Limit_Exceed") {
                        team.tlelast[problemname] = time;
                        team.protle = problemname;
                    }
                }
            } else {
                team.submitnumber1[problemname]++;
                if (team.flagac[problemname] == 1) {
                    if (submit_status == "Accepted") {
                        team.aclast[problemname] = time;
                        team.proac = problemname;
                    } else {
                        if (submit_status == "Wrong_Answer") {
                            team.walast[problemname] = time;
                            team.prowa = problemname;
                        }
                        if (submit_status == "Runtime_Error") {
                            team.relast[problemname] = time;
                            team.prore = problemname;
                        }
                        if (submit_status == "Time_Limit_Exceed") {
                            team.tlelast[problemname] = time;
                            team.protle = problemname;
                        }
                    }
                } else {
                    team.flagfrozen[problemname] = 1;
                    if (submit_status == "Accepted") {
                        team.aclast[problemname] = time;
                        team.proac = problemname;
                        team.solvednumber1[problemname]++;
                        team.timelist1[problemname] = time;
                        team.punishtime[problemname] =
                                (20 * team.beforefirstac[problemname] + time);
                        team.flagac[problemname] = 1;
                    } else {
                        team.beforefirstac[problemname]++;
                        if (submit_status == "Wrong_Answer") {
                            team.walast[problemname] = time;
                            team.prowa = problemname;
                        }
                        if (submit_status == "Runtime_Error") {
                            team.relast[problemname] = time;
                            team.prore = problemname;
                        }
                        if (submit_status == "Time_Limit_Exceed") {
                            team.tlelast[problemname] = time;
                            team.protle = problemname;
                        }
                    }
                }
            }
            realrank.insert(team_name);
        }

        if (order == "FLUSH") {
            int i = 1;
            for (auto iter = realrank.begin(); iter != realrank.end(); ++iter) {
                Information[*iter].rank = i;
                ++i;
            }
            cout << "[Info]Flush scoreboard.\n";
        }

        if (order == "FREEZE") {
            if (flagfreeze) {
                flagfreeze = false;
                cout << "[Info]Freeze scoreboard.\n";
            } else cout << "[Error]Freeze failed: scoreboard has been frozen.\n";
        }

        if (order == "SCROLL") {
            if (flagfreeze) {
                cout << "[Error]Scroll failed: scoreboard has not been frozen.\n";
            } else {
                cout << "[Info]Scroll scoreboard.\n";
                int k = 0;
                for (auto iter = realrank.begin(); iter != realrank.end(); ++iter) {
                    k++;
                    cout << *iter << ' ' << k << ' ' << Information[*iter].solvednumber << ' '
                         << Information[*iter].punishtimesum << ' ';                           //输出前面的榜单
                    Teaminfo &team = Information[*iter];
                    for (int i = 1; i <= problemcount; ++i) {
                        if (team.flagfrozen[i] == 0 && team.aclast[i] != 0) {
                            if (team.falsenumber[i] == 0) cout << '+';
                            else cout << '+' << team.falsenumber[i];
                        }
                        if (team.flagfrozen[i] == 0 && team.aclast[i] == 0) {
                            if (team.falsenumber[i] == 0) cout << '.';
                            else cout << '-' << team.falsenumber[i];
                        }
                        if (team.flagfrozen[i] == 1) {
                            if (team.falsenumber[i] == 0) cout << "0/" << team.submitnumber1[i];
                            else cout << '-' << team.falsenumber[i] << '/' << team.submitnumber1[i];
                            team.submitnumber1[i] = 0;
                        }
                        cout << ' ';
                    }
                    cout << endl;
                }
                string teamname1;
                for (auto iter = realrank.end(); iter != realrank.begin();) {
                    --iter;
                    teamname1 = *iter;
                    Teaminfo &teamm = Information[teamname1];
                    for (int i = 1; i <= problemcount; ++i) {
                        if (teamm.flagfrozen[i] == 1) {
                            auto last_pose = iter;
                            ++last_pose;
                            realrank.erase(*iter);
                            teamm.flagfrozen[i] = 0;
                            teamm.falsenumber[i] = teamm.beforefirstac[i];
                            if (teamm.flagac[i] == 1) {
                                teamm.punishtimesum += teamm.punishtime[i];
                                teamm.solvednumber += teamm.solvednumber1[i];
                                teamm.timelist[1] = teamm.timelist1[i];
                                sort(teamm.timelist, teamm.timelist + problemcount);
                                teamm.solvednumber1[i] = 0;
                                teamm.punishtime[i] = 0;
                                teamm.timelist1[i]=0;
                            }
                            auto iter1 = realrank.insert(teamname1).first;
                            auto replace_team = ++iter1;
                            if (replace_team != last_pose) {
                                cout << teamname1 << ' ' << *replace_team << ' ' << teamm.solvednumber << ' '
                                     << teamm.punishtimesum << endl;
                            }
                            iter = last_pose;
                            break;
                        }
                    }
                }
                int i = 0;
                for (auto iter = realrank.begin(); iter != realrank.end(); ++iter) {
                    i++;
                    cout << *iter << ' ' << i << ' ' << Information[*iter].solvednumber << ' '
                         << Information[*iter].punishtimesum << ' ';
                    Teaminfo &team = Information[*iter];
                    team.rank = i;
                    for (int j = 1; j <= problemcount; ++j) {
                        if (team.aclast[j] != 0) {
                            if (team.falsenumber[j] == 0) cout << '+';
                            else cout << '+' << team.falsenumber[j];
                        }
                        if (team.aclast[j] == 0) {
                            if (team.falsenumber[j] == 0) cout << '.';
                            else cout << '-' << team.falsenumber[j];
                        }
                        cout << ' ';
                    }
                    cout << endl;
                }
                flagfreeze = true;
            }
        }
        if (order == "QUERY_RANKING") {
            cin >> team_name;
            if (realrank.count(team_name) == 0)
                cout << "[Error]Query ranking failed: cannot find the team.\n";
            else {
                cout << "[Info]Complete query ranking.\n";
                if (!flagfreeze)
                    cout << "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.\n";
                cout << team_name << " NOW AT RANKING " << Information[team_name].rank << endl;
            }
        }
        if (order == "QUERY_SUBMISSION") {
            string temp1, temp2, status, problem;
            char problem_name;
            cin >> team_name >> temp1 >> problem >> temp2 >> status;  //
            if (realrank.count(team_name) == 0) cout << "[Error]Query submission failed: cannot find the team.\n";
            else {
                Teaminfo &team = Information[team_name];
                cout << "[Info]Complete query submission.\n";
                int problemname;
                if (status == "STATUS=Accepted") {
                    status = "Accepted";
                    if (problem == "PROBLEM=ALL") {
                        problemname = team.proac;
                        problem_name = static_cast<char>(problemname + 64);
                    } else {
                        problem_name = problem[8];
                        problemname = problem_name - 'A' + 1;
                    }
                    if (team.aclast[problemname] == 0) cout << "Cannot find any submission.\n";
                    else
                        cout << team_name << ' ' << problem_name << ' ' << status << ' ' << team.aclast[problemname]
                             << endl;
                }
                if (status == "STATUS=Wrong_Answer") {
                    status = "Wrong_Answer";
                    if (problem == "PROBLEM=ALL") {
                        problemname = team.prowa;
                        problem_name = static_cast<char>(problemname + 64);
                    } else {
                        problem_name = problem[8];
                        problemname = problem_name - 'A' + 1;
                    }
                    if (team.walast[problemname] == 0) cout << "Cannot find any submission.\n";
                    else
                        cout << team_name << ' ' << problem_name << ' ' << status << ' ' << team.walast[problemname]
                             << endl;
                }
                if (status == "STATUS=Runtime_Error") {
                    status = "Runtime_Error";
                    if (problem == "PROBLEM=ALL") {
                        problemname = team.prore;
                        problem_name = static_cast<char>(problemname + 64);
                    } else {
                        problem_name = problem[8];
                        problemname = problem_name - 'A' + 1;
                    }
                    if (team.relast[problemname] == 0) cout << "Cannot find any submission.\n";
                    else
                        cout << team_name << ' ' << problem_name << ' ' << status << ' ' << team.relast[problemname]
                             << endl;
                }
                if (status == "STATUS=Time_Limit_Exceed") {
                    status = "Time_Limit_Exceed";
                    if (problem == "PROBLEM=ALL") {
                        problemname = team.protle;
                        problem_name = static_cast<char>(problemname + 64);
                    } else {
                        problem_name = problem[8];
                        problemname = problem_name - 'A' + 1;
                    }
                    if (team.tlelast[problemname] == 0) cout << "Cannot find any submission.\n";
                    else
                        cout << team_name << ' ' << problem_name << ' ' << status << ' '
                             << team.tlelast[problemname]
                             << endl;
                }
                if (status == "STATUS=ALL") {
                    if (problem == "PROBLEM=ALL") {
                        problemname = team.proall;
                        problem_name = static_cast<char>(problemname + 64);
                        status=team.flagstatus[problemname];
                    } else {
                        problem_name = problem[8];
                        problemname = problem_name - 'A' + 1;
                        status=team.flagstatus[problemname];
                    }
                    if (team.submitlast[problemname] == 0) cout << "Cannot find any submission.\n";
                    else {
                        cout << team_name << ' ' << problem_name << ' ' << status << ' '
                             << team.submitlast[problemname]
                             << endl;
                    }
                }
            }
        }
        if (order == "END") break;
    }
    cout << "[Info]Competition ends.\n";
    return 0;
}