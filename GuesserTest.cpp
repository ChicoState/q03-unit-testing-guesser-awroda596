/**
 * Unit Tests for the class
**/

#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor)
};


//match testing
TEST(GuesserTest, correct_guess)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	iscorrect = guess.match("secret"); 
    ASSERT_EQ( true, iscorrect);
}

TEST(GuesserTest, simple_incorrect_guess)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	iscorrect = guess.match("terces"); 
    ASSERT_EQ( false, iscorrect);
}

TEST(GuesserTest, inverse_caps_incorrect)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	iscorrect = guess.match("SECRET"); 
    ASSERT_EQ( false, iscorrect);
}

TEST(GuesserTest, reversed_inverse_caps_incorrect)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("SECRET"); 
	iscorrect = guess.match("secret"); 
    ASSERT_EQ( false, iscorrect);
}

TEST(GuesserTest, handles_special)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("!@#$"); 
	iscorrect = guess.match("!@#$"); 
    ASSERT_EQ( true, iscorrect);
}


TEST(GuesserTest, lock_on_bruteforce_test)
{
	Guesser guess("secret"); 
	guess.match("aaaaa"); 
	bool r = guess.match("secret"); 
    ASSERT_EQ( false, r);
}

TEST(GuesserTest, prevent_correct_after_lock)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	guess.match("secrat");
	guess.match("secrat");
	guess.match("secrat");
	iscorrect = guess.match("secret"); 
    ASSERT_EQ( false, iscorrect);
}

//pass length testing

//password
TEST(GuesserTest, guess_longer_than_pass)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); //32 character password
	iscorrect = guess.match("secretaa"); 
    ASSERT_EQ( false, iscorrect);
}

//guess and pass are identical but past the length of accepted bounds.  should return false due to pass being past cap despite matching since pass is truncated
TEST(GuesserTest, length34_pass_and_identical_guess)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("123456789112345678921234567893123456"); //32 character password
	iscorrect = guess.match("1234567891123456789212345678931234"); //34 char pass, distance 2 = no lockout 
    ASSERT_EQ( false, iscorrect);
}

//guess is identical to truncated password
TEST(GuesserTest, length34_password_length32_identical_guess) 
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("12345678911234567892123456789312asdfadsfas"); //32 character password
	iscorrect = guess.match("12345678911234567892123456789312"); 
    ASSERT_EQ( true, iscorrect);
}

//bruteforce lockout on too long pass
TEST(GuesserTest, brute_force_lockout_longer_pass_Test) 
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); //32 character password
	guess.match("secretthisistoomanychars"); 
	iscorrect = guess.match("secret"); 
    ASSERT_EQ( false, iscorrect);
}
//remaining guesses tests

TEST(GuesserTest, 3_initial_guesses)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	rguesses = guess.remaining();
    ASSERT_EQ( 3, rguesses);
}

TEST(GuesserTest, 1_guesses_remaining)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	guess.match("secrat");
	guess.match("sacret");
	rguesses = guess.remaining();
    ASSERT_EQ( 1, rguesses);
}

TEST(GuesserTest, 2_guesses_remaining)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	guess.match("secrat");
	rguesses = guess.remaining();
    ASSERT_EQ( 2, rguesses);
}

TEST(GuesserTest, 0_guesses_remaining)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	guess.match("secrat");
	guess.match("secrat");
	guess.match("secrat");
	rguesses = guess.remaining();
    ASSERT_EQ( 0, rguesses);
}

TEST(GuesserTest, reset_guesses_from_initial){
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	guess.match("secret");
	rguesses = guess.remaining();
    ASSERT_EQ( 3, rguesses);
}

TEST(GuesserTest, reset_guesses_after_1_wrong){
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	guess.match("secrat");
	guess.match("secret");
	rguesses = guess.remaining();
    ASSERT_EQ( 3, rguesses);
}

//if bruteforced, locked. correct guess will not reset
TEST(GuesserTest, bruteforce_guesses_remaining)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	guess.match("aaaaa"); 
	iscorrect = guess.match("secret"); 
	rguesses = guess.remaining();
    ASSERT_EQ( 3, rguesses);
}

//if locked out, correct guess will not reset
TEST(GuesserTest, locked_out_remaining)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	guess.match("zecret"); 
	guess.match("zecret"); 
	guess.match("zecret"); 
	guess.match("secret"); 
	rguesses = guess.remaining();
    ASSERT_EQ( 0, rguesses);
}



/* template 
TEST(GuesserTest, name)
{
	bool iscorrect; 
	int rguesses; 
	Guesser guess("secret"); 
	guess.match("aaaaa"); 
	bool r = guess.match("secret");
    ASSERT_EQ( false, r);
}
*/