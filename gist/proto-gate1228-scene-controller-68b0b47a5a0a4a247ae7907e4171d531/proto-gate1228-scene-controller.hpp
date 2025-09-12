#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <list>

class Scene
{
public:
	int id;
	virtual ~Scene() = default;
private:
	sf::Clock clock_; //internal clock
	sf::RenderWindow& trgt_window_; // must always exist

public:
	//virtual method : this is where you write the scene code
	virtual void init()		= 0;
	virtual void input()	= 0;
	virtual void update()	= 0;
	virtual void render()	= 0;
	virtual void refresh()	= 0;

	explicit Scene(sf::RenderWindow& window):trgt_window_(window)
	{

	}

};

class SceneController
{
public:
	SceneController(){};
	~SceneController();

	using scene_ptr_list = std::list<Scene*>;

private:
	scene_ptr_list scenes_;
	std::unique_ptr<Scene> current_;

	//Scene Interface Methods
	void init_currrent()  const	{ current_->init(); }
	void input_current()  const	{ current_->input(); }
	void update_current() const	{ current_->update(); }
	void render_current() const	{ current_->render(); }

	//Access
	const Scene& first() { return *scenes_.front(); }
	const Scene& last() { return *scenes_.back(); }
	const Scene& get_by_id(int id) 
	{
		for (auto e : scenes_)
		{
			if (e->id == id)
				return *e;
		}
	}

	void next()					{ } //

	//Modify
	bool clear() { current_->refresh(); } //Restart scene without reinitialization
	bool erase() { current_.reset(std::move(scenes_.front())); };
};

