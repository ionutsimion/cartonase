#include <algorithm>
#include <cctype>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

namespace
{
    auto constexpr minimumNumberOfDigits{ 2 };
    auto constexpr maximumNumberOfDigits{ 200 };
    auto constexpr maximumNumberOfColumns{ 20 };

    static_assert(minimumNumberOfDigits >= 2 && maximumNumberOfDigits >= minimumNumberOfDigits);
}

int main()
{
    setlocale(LC_CTYPE,"");

    std::string keepPlaying{};
    do
    {
        std::cout << "Cu cate cartonase vrei sa joci (" << minimumNumberOfDigits << "<=n<=" << maximumNumberOfDigits
                  << ")? ";

        int numberOfDigits{};
        std::cin >> numberOfDigits;
        if (numberOfDigits < minimumNumberOfDigits || numberOfDigits > maximumNumberOfDigits)
            return 1;

        std::vector<int> digits(numberOfDigits);
        std::default_random_engine generator(std::chrono::steady_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> distribution(0, 9);
        std::generate(digits.begin(), digits.end(), [&distribution, &generator]
        {
            return distribution(generator);
        });

        std::cout << std::endl;

        auto const columns = std::min(numberOfDigits, maximumNumberOfColumns);

        std::cout << "   ";
        for (auto column = 0; column < columns; column++)
            std::cout << ' ' << std::left << std::setw(2) << column + 1;
        std::cout << std::endl;

        auto const rows = numberOfDigits / columns + (numberOfDigits % columns != 0);
        for (auto row = 0; row < rows; row++)
        {
            auto const actualColumns = row == rows - 1 ? numberOfDigits - (rows - 1) * columns : columns;
            std::cout << "   ";
            for (auto column = 0; column < actualColumns; column++)
                std::cout << "\u256d\u2500\u256e";
            std::cout << std::endl;
            std::cout << std::right << std::setw(2) << row + 1 << ' ';
            for (auto column = 0; column < actualColumns; column++)
                std::cout << "\u2502" << '?' << "\u2502";
            std::cout << std::endl;
            std::cout << "   ";
            for (auto column = 0; column < actualColumns; column++)
                std::cout << "\u2570\u2500\u256f";
            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Cate cifre vrei sa aiba numarul? ";
        int numberOfDigitsToTurn{};
        std::cin >> numberOfDigitsToTurn;

        std::cout << std::endl;

        std::cout << "Ce cartonase intorci (trebuie sa fie " << numberOfDigitsToTurn << ")? Scrie linia si coloana: ";

        std::set<std::pair<int, int>> turnedDigits{};
        for (auto i = 0; i < numberOfDigitsToTurn; i++)
        {
            int row{}, col{};
            std::cin >> row >> col;
            turnedDigits.emplace(row - 1, col - 1);
        }

        std::cout << std::endl;

        std::cout << "   ";
        for (auto column = 0; column < columns; column++)
            std::cout << ' ' << std::left << std::setw(2) << column + 1;
        std::cout << std::endl;

        for (auto row = 0; row < rows; row++)
        {
            auto const actualColumns = row == rows - 1 ? numberOfDigits - (rows - 1) * columns : columns;
            std::cout << "   ";
            for (auto column = 0; column < actualColumns; column++)
                std::cout << "\u256d\u2500\u256e";
            std::cout << std::endl;
            std::cout << std::right << std::setw(2) << row + 1 << ' ';
            for (auto column = 0; column < actualColumns; column++)
            {
                if (std::find(turnedDigits.cbegin(), turnedDigits.cend(), std::pair{row, column}) != turnedDigits.end())
                    std::cout << "\u2502" << digits[row * actualColumns + column] << "\u2502";
                else
                    std::cout << "\u2502" << ' ' << "\u2502";
            }
            std::cout << std::endl;
            std::cout << "   ";
            for (auto column = 0; column < actualColumns; column++)
                std::cout << "\u2570\u2500\u256f";
            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Care este cel mai mare numar ce se poate forma din cele " << numberOfDigitsToTurn << " cifre? ";

        std::string playerLargestNumber{};
        std::cin >> playerLargestNumber;

        std::vector<int> selectedDigits{};
        selectedDigits.reserve(numberOfDigitsToTurn);
        for (auto const &[row, column] : turnedDigits)
            selectedDigits.emplace_back(digits[row * columns + column]);
        std::sort(selectedDigits.begin(), selectedDigits.end(), std::greater<>());

        std::string largestNumberAsString;
        for (auto const d : selectedDigits)
            largestNumberAsString += static_cast<char>(d + '0');

        std::cout << std::endl;
        if (playerLargestNumber == largestNumberAsString)
            std::cout << "Felicitari, asa mi-a dat si mie :)." << std::endl;
        else
            std::cout << "Imi pare rau, dar ai gresit. Mie mi-a dat " << largestNumberAsString << '.' << std::endl;

        std::cout << std::endl;

        std::cout << "Mai joci? (da/nu): ";
        std::cin >> keepPlaying;
        std::transform(keepPlaying.begin(), keepPlaying.end(), keepPlaying.begin(), [](unsigned char c){ return std::tolower(c); });

        std::cout << std::endl;
    }
    while (keepPlaying == "da");

    return 0;
}
