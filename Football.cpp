#include <iostream>
#include <string>
#include <vector>

struct Player {
    int number;
    int efficiency;
    Player(int number, int efficiency) : number(number), efficiency(efficiency) {};
};

struct Team {
    std::vector<Player>::iterator begin;
    std::vector<Player>::iterator end;
    int64_t efficient;
};


bool CompareByEfficiency(const Player& first_player, const Player& second_player);
bool CompareByNumber(const Player& first_player, const Player& second_player);

std::vector<Player> ReadPlayers();

template<typename _iterator, typename compare>
_iterator Merge(_iterator first_player,
                _iterator middle_player,
                _iterator last_player,
                _iterator first_result,
                compare Compare);
 
template<typename _iterator, typename compare>
void MergeSort(_iterator first_player, _iterator last_player, compare Compare);


std::vector<Player> FindMostEffectiveSolidaryTeam(std::vector<Player> players);
int64_t FindTeamEfficiency(const std::vector<Player>& team);
void PrintTeam(std::vector<Player> result_team);


int main() {
    std::vector<Player> players = ReadPlayers();
    std::vector<Player> best_team = FindMostEffectiveSolidaryTeam(players);
    PrintTeam(best_team);
    return 0;
}


std::vector<Player> ReadPlayers() {
    std::vector<Player> players;
    int number_of_players;
    std::cin >> number_of_players;

    for (int index= 0; index < number_of_players; ++index) {
        int efficiency;
        std::cin >> efficiency;
        players.emplace_back(index + 1, efficiency);
    }
    return players;
}

bool CompareByEfficiency(const Player& first_player, const Player& second_player) {
    return first_player.efficiency < second_player.efficiency;
}

bool CompareByNumber(const Player& first_player, const Player& second_player) {
    return first_player.number < second_player.number;
}

template<typename _iterator, typename compare>
_iterator Merge(_iterator first_player,
                _iterator middle_player,
                _iterator last_player,
                _iterator first_result,
                compare Compare) {
    
    _iterator middle_player_right = middle_player;
    while (first_player != middle_player && middle_player_right != last_player) {
        if (Compare(*middle_player_right, *first_player)) {
            *first_result++ = *middle_player_right;
            ++middle_player_right;
        } else {
            *first_result++ = *first_player;
            ++first_player;
        }
    }
    while (first_player != middle_player) {
        *first_result++ = *first_player;
        ++first_player;
    }
    while (middle_player_right != last_player) {
        *first_result++ = *middle_player_right;
        ++middle_player_right;
    }
    
    return first_result;
}

template<typename _iterator, typename compare>
void MergeSort(_iterator first_player, _iterator last_player, compare Compare) {
    auto size = std::distance(first_player, last_player);
    if (size < 2) {
        return;
    }
    _iterator middle_player = std::next(first_player, size / 2);
    MergeSort(first_player, middle_player, Compare);
    MergeSort(middle_player, last_player, Compare);
    
    std::vector<typename std::iterator_traits<_iterator>::value_type> buf;
    buf.reserve(size);
    
    _iterator buf_end = Merge(first_player, middle_player, last_player, buf.begin(), Compare);
    std::move(buf.begin(), buf_end, first_player);
}


std::vector<Player> FindMostEffectiveSolidaryTeam(std::vector<Player> players) {
    
    if (players.size() <= 2) {
        return players;
    }
    
    MergeSort(players.begin(), players.end(), CompareByEfficiency);

    auto best_player = std::next(players.begin(), 1);
    auto worst_player = players.begin();
    
    int64_t best_player_ef = best_player->efficiency;
    int64_t worst_player_ef = worst_player->efficiency;
    
    int64_t cur_efficient = best_player_ef + worst_player_ef;
    
    Team current_team = {worst_player, best_player, cur_efficient};
    Team max_team = current_team;
    
    for (auto right_player = std::next(players.begin(), 2);
         right_player != players.end();
         ++right_player) {
        
        int64_t first_bad = current_team.begin->efficiency;
        int64_t second_bad = (current_team.begin + 1)->efficiency;
        int64_t best_cur_player = right_player->efficiency;
        
        while (std::distance(current_team.begin, right_player) >= 2 &&
               first_bad + second_bad < best_cur_player) {
            current_team.efficient -= first_bad;
            current_team.begin++;
            first_bad = current_team.begin->efficiency;
            second_bad = (current_team.begin + 1)->efficiency;
        }
        current_team.end = right_player + 1;
        current_team.efficient += best_cur_player;
        if (max_team.efficient < current_team.efficient) {
            max_team = current_team;
        }
    }
    
    return {max_team.begin, max_team.end};
}

int64_t FindTeamEfficiency(const std::vector<Player>& team) {
    int64_t sum_efficiency = 0;
    for (const auto& player : team) {
        sum_efficiency += player.efficiency;
    }
    
    return sum_efficiency;
}

void PrintTeam(std::vector<Player> result_team) {
    std::cout << FindTeamEfficiency(result_team) << std::endl;
    
    MergeSort(result_team.begin(), result_team.end(), CompareByNumber);
    for (const auto& player : result_team) {
        std::cout << player.number << " ";
    }
    std::cout << std::endl;
}
