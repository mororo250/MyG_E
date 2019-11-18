#pragma once

class Renderer
{
public:
	Renderer();
	virtual ~Renderer() = default;
	 
	virtual void draw_element(class IndexBuffer const& ib) const = 0;
	virtual void clear() const = 0;

	virtual void set_active() {}
	virtual void set_disactive() {}

	inline bool has_init() { return m_is_active; }

protected:
	bool m_is_active;
};
