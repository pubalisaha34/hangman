#include <iostream>
// #include <ctime> // removed this is not needed when using <random> header
#include <random>   // prefered random generator for C++, srand is more of a C left over.
#include <string>
#include <vector>

//---------------------------------------------------------------------------------------------------------------------
// best to put everything related to the game in a class

class Hangman final // this class is not supposed to be extended
{
    const unsigned int maximum_number_of_tries = 6;                     // to avoid "magic" constants in code

public:
    explicit Hangman(const std::vector<std::string>& words) :           // ANY constructor with only one parameter should be explicit (to avoid it being used as implicit type converter)
        m_random_generator{ std::random_device{}() },                   // initialize random generator C++ style
        m_distribution{ 0, words.size() - 1 },                          // initialize a uniform distribution with a range
        m_word_number{ m_distribution(m_random_generator) },            // extra variable for debugability
        m_secret_word{ words[m_word_number] },                          // pick a random word from the input
        m_guess_word{ std::string(m_secret_word.length(),'_') },        // initialize secret word with '_'
        m_try_no{ 0u }                                                  // consider renaming to "m_round"
    {
    }

    // todo : document
    void play()
    {
        bool won{ false };
        bool lost{ false };

        std::cout << "Welcome to Hangman!\n";

        // next line reads better then while true
        // good code documents itself, it tells a story.
        // so also use "small" functions to help
        // the code explain itself
        while (!won && !lost)
        {
            show_state();
            auto guess = make_guess();              // by making this a function you can easily add error checking later without changing the flow here.
            handle_guess(guess);                    // process the guess made by the user by updating the internal state of the guessed word.
            won = check_win();                      
            if (!won) lost = check_loss();
        }
    }

private:
    // static member function, no member variables are used
    static char make_guess()
    {
        char c;
        std::cout << "\nEnter your guess:\n";
        std::cin >> c;

        // todo check if c is a valid letter!
        return c;
    }

    // const member function, no member variables will be changed
    void show_state() const                                             
    {
        std::cout << "\n\n";
        std::cout << m_hangman_art[m_try_no] << "\n";
        std::cout << m_guess_word << "\n";
    }

    // cannot be a const method since members are modified.
    void handle_guess(char guess)
    {
        if (m_secret_word.find(guess) != std::string::npos)
        {
            for (std::size_t i = 0; i < m_guess_word.length(); ++i) // changed int to size_t
            {
                if (m_secret_word[i] == guess)
                {
                    m_guess_word[i] = guess;
                }
            }
        }
    
        // increase try number
        ++m_try_no;
    }

    bool check_win() const
    {
        if (m_secret_word != m_guess_word) return false;
    
        std::cout << m_hangman_art[m_try_no] << "\n";
        std::cout << m_guess_word << "\n";
        std::cout << "You win! The word was " << m_secret_word << "\n";
        return true;
    }

    bool check_loss() const
    {
        if (m_try_no < maximum_number_of_tries) return false;           

        std::cout << m_hangman_art[m_try_no] << "\n";
        std::cout << m_guess_word << "\n";
        std::cout << "You lost! The word was " << m_secret_word << "\n";
        return true;
    }

    std::mt19937 m_random_generator;
    std::uniform_int_distribution<std::size_t> m_distribution;
    std::size_t m_word_number;
    std::string m_secret_word;
    std::string m_guess_word;
    unsigned int m_try_no;

    // Credit for art - https://gist.github.com/chrishorton/8510732aa9a80a03c829b09f12e20d9c
    // static because this art is the same for all instances of Hangman
    const std::string m_hangman_art[7] = {
        " +---+\n"
        " |   |\n"
        "     |\n"
        "     |\n"
        "     |\n"
        "     |\n"
        "==========",
        " +---+\n"
        " |   |\n"
        " O   |\n"
        "     |\n"
        "     |\n"
        "     |\n"
        "==========",
        " +---+\n"
        " |   |\n"
        " O   |\n"
        " |   |\n"
        "     |\n"
        "     |\n"
        "==========",
        " +---+\n"
        " |   |\n"
        " O   |\n"
        "/|   |\n"
        "     |\n"
        "     |\n"
        "==========",
        " +---+\n"
        " |   |\n"
        " O   |\n"
        "/|\\  |\n"
        "     |\n"
        "     |\n"
        "==========",
        " +---+\n"
        " |   |\n"
        " O   |\n"
        "/|\\  |\n"
        "/    |\n"
        "     |\n"
        "==========",
        " +---+\n"
        " |   |\n"
        " O   |\n"
        "/|\\  |\n"
        "/ \\  |\n"
        "     |\n"
        "=========="
    };
};

//---------------------------------------------------------------------------------------------------------------------

int main()
{
    const std::vector<std::string> words = { "programming", "hangman", "games" };
    Hangman game{ words };
    game.play();
    return 0;
}