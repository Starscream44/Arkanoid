#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include <assert.h>

namespace SnakeGame
{
	void InitGameStateMainMenu(GameStateMainMenuData& data)
	{
		assert(data.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		MenuItem startGame;
		startGame.SetTextString("Start Game");
		startGame.SetTextFont(data.font);
		startGame.SetTextCharacterSize(24);
		startGame.SetOnPressCallback([](MenuItem&) {
			Application::Instance().GetGame().SwitchStateTo(GameStateType::Playing);
			});

		const bool isInfiniteApples = Application::Instance().GetGame().IsEnableOptions(GameOptions::InfiniteApples);
		MenuItem optionsInfiniteApplesItem;
		optionsInfiniteApplesItem.SetTextString("Infinite Apples: " + std::string(isInfiniteApples ? "On" : "Off"));
		optionsInfiniteApplesItem.SetTextFont(data.font);
		optionsInfiniteApplesItem.SetTextCharacterSize(24);
		optionsInfiniteApplesItem.SetOnPressCallback([](MenuItem& item) {
			Game& game = Application::Instance().GetGame();
			bool newOptionValue = !game.IsEnableOptions(GameOptions::InfiniteApples);
			game.SetOption(GameOptions::InfiniteApples, newOptionValue);
			item.SetTextString("Infinite Apples: " + std::string(newOptionValue ? "On" : "Off"));
			});

		const bool isWithAcceleration = Application::Instance().GetGame().IsEnableOptions(GameOptions::WithAcceleration);
		MenuItem optionsWithAccelerationItem;
		optionsWithAccelerationItem.SetTextString("With Acceleration: " + std::string(isWithAcceleration ? "On" : "Off"));
		optionsWithAccelerationItem.SetTextFont(data.font);
		optionsWithAccelerationItem.SetTextCharacterSize(24);
		optionsWithAccelerationItem.SetOnPressCallback([](MenuItem& item) {
			Game& game = Application::Instance().GetGame();
			bool newOptionValue = !game.IsEnableOptions(GameOptions::WithAcceleration);
			game.SetOption(GameOptions::WithAcceleration, newOptionValue);
			item.SetTextString("With Acceleration: " + std::string(newOptionValue ? "On" : "Off"));
			});

		MenuItem options;
		options.SetTextString("Options");
		options.SetTextFont(data.font);
		options.SetTextCharacterSize(24);
		options.SetHintString("Options");
		options.SetHintFont(data.font);
		options.SetHintCharacterSize(48);
		options.SetHintFillColor(sf::Color::Red);
		options.SetChildrenLayout(Orientation::Vertical, Alignment::Middle, 10.f);
		options.AddChild(optionsInfiniteApplesItem);
		options.AddChild(optionsWithAccelerationItem);

		MenuItem recordsItem;
		recordsItem.SetTextString("Records");
		recordsItem.SetTextFont(data.font);
		recordsItem.SetTextCharacterSize(24);
		recordsItem.SetOnPressCallback([](MenuItem&) {
			Application::Instance().GetGame().PushState(GameStateType::Records, true);
			});

		MenuItem yesItem;
		yesItem.SetTextString("Yes");
		yesItem.SetTextFont(data.font);
		yesItem.SetTextCharacterSize(24);
		yesItem.SetOnPressCallback([](MenuItem&) {
			Application::Instance().GetGame().SwitchStateTo(GameStateType::None);
			});

		MenuItem noItem;
		noItem.SetTextString("No");
		noItem.SetTextFont(data.font);
		noItem.SetTextCharacterSize(24);
		noItem.SetOnPressCallback([&data](MenuItem&) {
			data.menu.GoBack();
			});

		MenuItem exitGameItem;
		exitGameItem.SetTextString("Exit Game");
		exitGameItem.SetTextFont(data.font);
		exitGameItem.SetTextCharacterSize(24);
		exitGameItem.SetHintString("Are you sure?");
		exitGameItem.SetHintFont(data.font);
		exitGameItem.SetHintCharacterSize(48);
		exitGameItem.SetHintFillColor(sf::Color::Red);
		exitGameItem.SetChildrenLayout(Orientation::Horizontal, Alignment::Middle, 10.f);
		exitGameItem.AddChild(yesItem);
		exitGameItem.AddChild(noItem);

		MenuItem mainMenu;
		mainMenu.SetHintString("Snake Game");
		mainMenu.SetHintFont(data.font);
		mainMenu.SetHintCharacterSize(48);
		mainMenu.SetHintFillColor(sf::Color::Red);
		mainMenu.SetChildrenLayout(Orientation::Vertical, Alignment::Middle, 10.f);
		mainMenu.AddChild(startGame);
		mainMenu.AddChild(options);
		mainMenu.AddChild(recordsItem);
		mainMenu.AddChild(exitGameItem);

		data.menu.Init(mainMenu);
	}

	void ShutdownGameStateMainMenu(GameStateMainMenuData& data)
	{
		// No need to do anything here
	}

	void HandleGameStateMainMenuWindowEvent(GameStateMainMenuData& data, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				data.menu.GoBack();
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				data.menu.PressOnSelectedItem();
			}
			
			Orientation orientation = data.menu.GetCurrentContext().GetChildrenOrientation();
			if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left)
			{
				data.menu.SwitchToPreviousMenuItem();
			}
			else if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down ||
						orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right)
			{
				data.menu.SwitchToNextMenuItem();
			}
		}
	}

	void UpdateGameStateMainMenu(GameStateMainMenuData& data, float timeDelta)
	{

	}

	void DrawGameStateMainMenu(GameStateMainMenuData& data, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		sf::Text hintText = data.menu.GetCurrentContext().GetHintText();
		hintText.setOrigin(GetTextOrigin(hintText, { 0.5f, 0.f }));
		hintText.setPosition(viewSize.x / 2.f, 150.f);
		window.draw(hintText);

		data.menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
	}

}
