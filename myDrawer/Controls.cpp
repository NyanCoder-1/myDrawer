#include "pch.h"

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};
struct Const
{
	XMMATRIX WVP;
};
#include "Controls.h"
#include "MainRender.h"


Event& operator+=(Event& left, const function<void()> right)
{
	left.push_back(right);
	return left;
}
Controls& operator+=(Controls& left, Control* right)
{
	left.push_back(right);
	return left;
}

Control::Control(MainRender *render)
{
	m_render = render;
	x = 0; y = 0; width = 0; height = 0; visible = true;
}


BMButton::BMButton(MainRender *render) :Control(render)
{}

void BMButton::SetImg(wchar_t *imgstr, float width, float height)
{
	m_render->TurnOnAlphaBlending();
	m_img = new Image(m_render);
	m_img->Init(imgstr, width, height);
	SetSize(width, height);
}
void BMButton::Draw()
{
	if (!visible)
		return;
	XMFLOAT2 size = m_img->GetSize();
	m_img->Draw(x + width / 2 - size.x / 2, y + height / 2 - size.y / 2);
}
bool BMButton::MouseDown(const MouseEventClick &arg)
{
	if (!visible)
		return false;
	for (function<void()> f : OnClick)
		f();
	return true;
}


Button::Button(MainRender *render) :Control(render)
{
	m_imgs.push_back(new Image(render));
	m_imgs[0]->Init(L"data/images/button1.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[1]->Init(L"data/images/button2.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[2]->Init(L"data/images/button3.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[3]->Init(L"data/images/button4.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[4]->Init(L"data/images/button5.png", 10, 1);
	m_imgs.push_back(new Image(render));
	m_imgs[5]->Init(L"data/images/button6.png", 10, 1);
	m_imgs.push_back(new Image(render));
	m_imgs[6]->Init(L"data/images/button7.png", 1, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[7]->Init(L"data/images/button8.png", 1, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[8]->Init(L"data/images/button9.png", 1, 1);
	m_pfont = new BitmapFont(render);
	m_pfont->Init("Segoe UI.fnt");
	m_ptxt = new Text(render, m_pfont);
	m_ptxt->Init(L"", false, 255);
}

void Button::SetCaption(const wstring text)
{
	m_ptxt->SetText(text);
}
void Button::Draw()
{
	if (!visible)
		return;
	m_imgs[0]->Draw(x, y);
	m_imgs[1]->Draw(x + width - 10, y);
	m_imgs[2]->Draw(x, y + height - 10);
	m_imgs[3]->Draw(x + width - 10, y + height - 10);
	m_imgs[4]->SetSize(10, height - 20);
	m_imgs[4]->Draw(x, y + 10);
	m_imgs[5]->SetSize(10, height - 20);
	m_imgs[5]->Draw(x + width - 10, y + 10);
	m_imgs[6]->SetSize(width - 20, 10);
	m_imgs[6]->Draw(x + 10, y);
	m_imgs[7]->SetSize(width - 20, 10);
	m_imgs[7]->Draw(x + 10, y + height - 10);
	m_imgs[8]->SetSize(width - 20, height - 20);
	m_imgs[8]->Draw(x + 10, y + 10);
	m_ptxt->Draw(1.0f, 1.0f, 1.0f, x + 5, y + height / 2 - 10);
}
bool Button::MouseDown(const MouseEventClick &arg)
{
	if (!visible)
		return false;
	for (function<void()> f : OnClick)
		f();
	return true;
}


HidingTab::HidingTab(MainRender *render) :Control(render)
{
	m_imgs.push_back(new Image(render));
	m_imgs[0]->Init(L"data/images/ht_btn.png", 35, 15);
	m_imgs.push_back(new Image(render));
	m_imgs[1]->Init(L"data/images/ht_color.png", 1, 1);
	m_imgs.push_back(new Image(render));
	m_imgs[2]->Init(L"data/images/ht_shade1.png", 1, 5);
	m_imgs.push_back(new Image(render));
	m_imgs[3]->Init(L"data/images/ht_shade2.png", 5, 5);
	m_imgs.push_back(new Image(render));
	m_imgs[4]->Init(L"data/images/ht_arrowDown.png", 35, 15);
	m_imgs.push_back(new Image(render));
	m_imgs[5]->Init(L"data/images/ht_arrowUp.png", 35, 15);
	m_isHide = false;
}
void HidingTab::Draw()
{
	if (!visible)
		return;
	if (!m_isHide)
	{
		m_imgs[0]->Draw(x, y + height - 15);
		m_imgs[5]->Draw(x, y + height - 15);
	}
	else
	{
		m_imgs[0]->Draw(x, y);
		m_imgs[4]->Draw(x, y);
		m_imgs[1]->SetSize(width, height - 15);
		m_imgs[1]->Draw(x, y + 15);
		m_imgs[2]->SetSize(width - 35, 5);
		m_imgs[2]->Draw(x + 35, y + 15);
		m_imgs[3]->Draw(x + 30, y + 15);
		for (Control *control : Controls)
		{
			control->x += x;
			control->y += y + 15;
			control->Draw();
			control->x -= x;
			control->y -= y + 15;
		}
	}
}
bool HidingTab::MouseDown(const MouseEventClick &arg)
{
	if (!m_isHide)
	{
		if (in(arg.x, x, x + 34) && in(arg.y, y + height - 15, y + height - 1))
		{
			m_isHide = true;
			m_render->UpdatePos();
			return true;
		}
	}
	else {
		if (in(arg.x, x, x + 34) && in(arg.y, y, y + 14))
		{
			m_isHide = false;
			m_render->UpdatePos();
			return true;
		}
		if (in(arg.x, x, x + width - 1) && in(arg.y, y + 15, y + height - 1))
		{
			for (Control *control : Controls)
			{
				MouseEventClick arg1 = arg;
				arg1.x -= x;
				arg1.y -= y + 15;
				if (in(arg1.x, control->x, control->x + control->width - 1) && in(arg1.y, control->y, control->y + control->height - 1) && control->MouseDown(arg1))
					return true;
			}
			return true;
		}
	}
	return false;
}


CustomImgRadioButtons::CustomImgRadioButtons(MainRender *render) :Control(render)
{
	m_imgs.push_back(new Image(render));
	m_imgs[0]->Init(L"data/images/button1.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[1]->Init(L"data/images/button2.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[2]->Init(L"data/images/button3.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[3]->Init(L"data/images/button4.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[4]->Init(L"data/images/button5.png", 10, 20);
	m_imgs.push_back(new Image(render));
	m_imgs[5]->Init(L"data/images/button6.png", 10, 20);
	m_imgs.push_back(new Image(render));
	m_imgs[6]->Init(L"data/images/button7.png", 20, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[7]->Init(L"data/images/button8.png", 20, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[8]->Init(L"data/images/button9.png", 20, 20);
	m_chosen = 0;
}
void CustomImgRadioButtons::Draw()
{
	if (!visible)
		return;

	x += m_chosen * 50 + 5;
	y += 5;
	m_imgs[0]->Draw(x, y);
	m_imgs[1]->Draw(x + 30, y);
	m_imgs[2]->Draw(x, y + 30);
	m_imgs[3]->Draw(x + 30, y + 30);
	m_imgs[4]->Draw(x, y + 10);
	m_imgs[5]->Draw(x + 30, y + 10);
	m_imgs[6]->Draw(x + 10, y);
	m_imgs[7]->Draw(x + 10, y + 30);
	m_imgs[8]->Draw(x + 10, y + 10);
	x -= m_chosen * 50 + 5;
	y -= 5;
	for (int i = 9; i < m_imgs.size(); i++)
	{
		m_imgs[i]->Draw(x + (i - 9) * 50 + 5, y + 5);
	}
}
bool CustomImgRadioButtons::MouseDown(const MouseEventClick &arg)
{
	if (!visible)
		return false;
	for (int i = 0; i < m_imgs.size() - 9; i++)
		if (in(arg.x, x + i * 50 + 5, x + i * 50 + 44) && in(arg.y, y + 5, y + 44))
		{
			m_chosen = i;
			OnClick[i]();
			return true;
		}
	return false;
}

void CustomImgRadioButtons::AddButton(wstring imagename, function<void()> onClick)
{
	m_imgs.push_back(new Image(m_render));
	m_imgs[m_imgs.size() - 1]->Init(imagename.c_str(), 40, 40);
	OnClick += onClick;
}


HScrollBar::HScrollBar(MainRender *render) :Control(render)
{
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs[0]->Init(L"data/images/SBBtn.png", 10, 10);
	m_imgs[1]->Init(L"data/images/ScrollLeft.png", 5, 10);
	m_imgs[2]->Init(L"data/images/ScrollRight.png", 5, 10);
	m_imgs[3]->Init(L"data/images/ScrollHorizontal.png", 1, 10);
	m_imgs[4]->Init(L"data/images/SBArrowLeft.png", 10, 10);
	m_imgs[5]->Init(L"data/images/SBArrowRight.png", 10, 10);
	pSize = 1;
	min = 0;
	max = 0;
}
HScrollBar::~HScrollBar()
{
	for (Image *i : m_imgs)
		_CLOSE(i);
}
void HScrollBar::Draw()
{
	wstringstream str;

	if (!visible)
		return;
	m_imgs[0]->Draw(x, y);
	m_imgs[0]->Draw(x + width - 10, y);
	m_imgs[4]->Draw(x, y);
	m_imgs[5]->Draw(x + width - 10, y);
	if (max <= min)
	{
		m_imgs[3]->SetSize(width - 30, 10);
		m_imgs[1]->Draw(x + 10, y);
		m_imgs[2]->Draw(x + width - 15, y);
		m_imgs[3]->Draw(x + 15, y);
	}
	else
	{
		float size = (width - 20)*pSize;

		if (pSize > 1.0f)
		{
			size = width - 20;
			left = 0;
		}
		else
		{
			if (size < 10)
				size = 10;

			left = (width - 20 - size) * (*value - min) / (max - min);
			if (*value < min)
			{
				left = 0;
			}
			else
				if (*value > max)
				{
					left = width - 20 - size;
				}
		}
		right = left + size;

		m_imgs[3]->SetSize((int)(right - left - 9), 10);
		m_imgs[1]->Draw((int)(x + left + 10), (int)y);
		m_imgs[2]->Draw((int)(x + right + 5), (int)y);
		m_imgs[3]->Draw((int)(x + left + 15), (int)y);
	}
}
bool HScrollBar::MouseDown(const MouseEventClick &arg)
{
	if (!visible)
		return false;
	if (in(arg.y, y, y + height))
	{
		if (in(arg.x, x, x + 9))
		{
			*value -= (float(5) / (width - right + left - 20))*(max - min);
			return true;
		}
		if (in(arg.x, x + width - 10, x + width - 1))
		{
			*value += (float(5) / (width - right + left - 20))*(max - min);
			return true;
		}
		if (in(arg.x, x + left + 10, x + right + 10))
		{
			value0 = *value;
			downx = arg.x;
			IsMoving = true;
			SetCapture(D3D11Framework::Window::Get()->GetHWND());
			return true;
		}
	}
	return false;
}
bool HScrollBar::MouseUp(const MouseEventClick &arg)
{
	if (IsMoving)
	{
		IsMoving = false;
		ReleaseCapture();
		return true;
	}
	return false;
}
bool HScrollBar::MouseMove(const MouseEvent &arg)
{
	if (IsMoving)
	{
		*value = value0 + (float(arg.x - downx) / (width - right + left - 20))*(max - min);
		if (*value < min)
			*value = min;
		if (*value > max)
			*value = max;
		return true;
	}
	return false;
}


VScrollBar::VScrollBar(MainRender *render) :Control(render)
{
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs.push_back(new Image(render));
	m_imgs[0]->Init(L"data/images/SBBtn.png", 10, 10);
	m_imgs[1]->Init(L"data/images/ScrollUp.png", 10, 5);
	m_imgs[2]->Init(L"data/images/ScrollDown.png", 10, 5);
	m_imgs[3]->Init(L"data/images/ScrollVertical.png", 10, 1);
	m_imgs[4]->Init(L"data/images/SBArrowUp.png", 10, 10);
	m_imgs[5]->Init(L"data/images/SBArrowDown.png", 10, 10);
	pSize = 1;
	min = 0;
	max = 0;
}
VScrollBar::~VScrollBar()
{
	for (Image *i : m_imgs)
		_CLOSE(i);
}
void VScrollBar::Draw()
{
	if (!visible)
		return;
	m_imgs[0]->Draw(x, y);
	m_imgs[0]->Draw(x, y + height - 10);
	m_imgs[4]->Draw(x, y);
	m_imgs[5]->Draw(x, y + height - 10);
	if (max <= min)
	{
		m_imgs[3]->SetSize(10, height - 30);
		m_imgs[1]->Draw(x, y + 10);
		m_imgs[2]->Draw(x, y + height - 15);
		m_imgs[3]->Draw(x, y + 15);
	}
	else
	{
		float size = (height - 20)*pSize;

		if (pSize > 1.0f)
		{
			size = height - 20;
			up = 0;
		}
		else
		{
			if (size < 10)
				size = 10;

			up = (height - 20 - size) * (*value - min) / (max - min);
			if (*value < min)
			{
				up = 0;
			}
			else
				if (*value > max)
				{
					up = height - 20 - size;
				}
		}
		down = up + size;

		m_imgs[3]->SetSize(10, (int)(down - up - 9));
		m_imgs[1]->Draw((int)x, (int)(y + up + 10));
		m_imgs[2]->Draw((int)x, (int)(y + down + 5));
		m_imgs[3]->Draw((int)x, (int)(y + up + 15));
	}
}
bool VScrollBar::MouseDown(const MouseEventClick &arg)
{
	if (!visible)
		return false;
	if (in(arg.x, x, x + width))
	{
		if (in(arg.y, y, y + 9))
		{
			*value -= (float(5) / (height - down + up - 20))*(max - min);
			return true;
		}
		if (in(arg.y, y + height - 10, y + height - 1))
		{
			*value += (float(5) / (height - down + up - 20))*(max - min);
			return true;
		}
		if (in(arg.y, y + up + 10, y + down + 10))
		{
			value0 = *value;
			downy = arg.y;
			IsMoving = true;
			SetCapture(D3D11Framework::Window::Get()->GetHWND());
			return true;
		}
	}
	return false;
}
bool VScrollBar::MouseUp(const MouseEventClick &arg)
{
	if (IsMoving)
	{
		IsMoving = false;
		ReleaseCapture();
		return true;
	}
	return false;
}
bool VScrollBar::MouseMove(const MouseEvent &arg)
{
	if (IsMoving)
	{
		*value = value0 + (float(arg.y - downy) / (height - down + up - 20))*(max - min);
		if (*value < min)
			*value = min;
		if (*value > max)
			*value = max;
		return true;
	}
	return false;
}

HoverBox::HoverBox(MainRender *render) :Control(render)
{
	m_rect = new D3D11Framework::Rectangle(render);
	m_rect->Init(render->m_width, render->m_height);
	m_rect->SetColor(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));
	m_imgs.push_back(new Image(render));
	m_imgs[0]->Init(L"data/images/button1.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[1]->Init(L"data/images/button2.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[2]->Init(L"data/images/button3.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[3]->Init(L"data/images/button4.png", 10, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[4]->Init(L"data/images/button5.png", 10, 1);
	m_imgs.push_back(new Image(render));
	m_imgs[5]->Init(L"data/images/button6.png", 10, 1);
	m_imgs.push_back(new Image(render));
	m_imgs[6]->Init(L"data/images/button7.png", 1, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[7]->Init(L"data/images/button8.png", 1, 10);
	m_imgs.push_back(new Image(render));
	m_imgs[8]->Init(L"data/images/button9.png", 1, 1);
}

void HoverBox::Draw()
{
	m_rect->Draw(0, 0);
	int x = m_render->m_width / 2 - width / 2, y = m_render->m_height / 2 - height / 2;
	m_imgs[0]->Draw(x, y);
	m_imgs[1]->Draw(x + width - 10, y);
	m_imgs[2]->Draw(x, y + height - 10);
	m_imgs[3]->Draw(x + width - 10, y + height - 10);
	m_imgs[4]->SetSize(10, height - 20);
	m_imgs[4]->Draw(x, y + 10);
	m_imgs[5]->SetSize(10, height - 20);
	m_imgs[5]->Draw(x + width - 10, y + 10);
	m_imgs[6]->SetSize(width - 20, 10);
	m_imgs[6]->Draw(x + 10, y);
	m_imgs[7]->SetSize(width - 20, 10);
	m_imgs[7]->Draw(x + 10, y + height - 10);
	m_imgs[8]->SetSize(width - 20, height - 20);
	m_imgs[8]->Draw(x + 10, y + 10);
	for (Control *control : m_controls)
	{
		control->x += x;
		control->y += y;
		control->Draw();
		control->x -= x;
		control->y -= y;
	}
}
bool HoverBox::MouseDown(const MouseEventClick &arg)
{
	if (visible)
	{
		MouseEventClick arg1 = arg;
		int x = m_render->m_width / 2 - width / 2, y = m_render->m_height / 2 - height / 2;
		arg1.x -= x;
		arg1.y -= y;
		for (int i = m_controls.size() - 1; i >= 0; i--)
		{
			auto c = m_controls[i];
			if (c->IsVisible())
				if (c->MouseDown(arg1))
					return true;
		}
		return true;
	}
	else return false;
}
bool HoverBox::MouseUp(const MouseEventClick &arg)
{
	if (visible)
	{
		MouseEventClick arg1 = arg;
		int x = m_render->m_width / 2 - width / 2, y = m_render->m_height / 2 - height / 2;
		arg1.x -= x;
		arg1.y -= y;
		for (int i = m_controls.size() - 1; i >= 0; i--)
		{
			auto c = m_controls[i];
			if (c->IsVisible())
				if (c->MouseUp(arg1))
					return true;
		}
		return true;
	}
	else return false;
}
bool HoverBox::MouseMove(const MouseEvent &arg)
{
	if (visible)
	{
		MouseEvent arg1 = arg;
		int x = m_render->m_width / 2 - width / 2, y = m_render->m_height / 2 - height / 2;
		arg1.x -= x;
		arg1.y -= y;
		for (int i = m_controls.size() - 1; i >= 0; i--)
		{
			auto c = m_controls[i];
			if (c->IsVisible())
				if (c->MouseMove(arg1))
					return true;
		}
		return true;
	}
	else return false;
}

ColoredRectButton::ColoredRectButton(MainRender * render) :Control(render)
{
	m_rect = new D3D11Framework::Rectangle(render);
	m_rect->Init(10, 10);
}

void ColoredRectButton::SetColor(XMFLOAT4 color)
{
	m_rect->SetColor(color);
}
void ColoredRectButton::Draw()
{
	if ((m_rect->GetSize().x != width) || (m_rect->GetSize().y != height))
		m_rect->SetSize(width, height);
	m_rect->Draw(x, y);
}
bool ColoredRectButton::MouseDown(const MouseEventClick &arg)
{
	if (in(arg.x, x, x + width) && in(arg.y, y, y + height)) {
		m_isClicking = true;
		return true;
	}
	return false;
}
bool ColoredRectButton::MouseUp(const MouseEventClick &arg)
{
	if (m_isClicking)
	{
		if (in(arg.x, x, x + width) && in(arg.y, y, y + height))
			for (function<void()> f : OnClick)
				f();
		m_isClicking = false;
		return true;
	}
	return false;
}