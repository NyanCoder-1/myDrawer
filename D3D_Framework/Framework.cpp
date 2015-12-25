#include "stdafx.h"
#include "Framework.h"
#include "macros.h"
#include "Log.h"

namespace D3D11Framework
{
//------------------------------------------------------------------

	Framework::Framework() :
		m_wnd(nullptr),
		m_render(nullptr),
		m_input(nullptr),
		m_init(false)
	{
	}

	Framework::~Framework()
	{
	}

	void Framework::AddInputListener(InputListener *listener)
	{
		if (m_input)
			m_input->AddListener(listener);
	}

	bool Framework::Init(const FrameworkDesc &desc)
	{
		m_render = desc.render;

		m_wnd = new Window();
		m_input = new InputMgr();

		m_input->Init();

		if (!m_wnd->Create(desc.wnd))
		{
			Log::Get()->Err("�� ������� ������� ����");
			return false;
		}
		m_wnd->SetInputMgr(m_input);

		if (!m_render->CreateDevice(m_wnd->GetHWND()))
		{
			Log::Get()->Err("�� ������� ������� ������");
			return false;
		}

		m_init = true;
		return true;
	}

	void Framework::Run()
	{
		// Yeah! ������� ���� �����������! � ���� ���������� ���������� ��������� ������������ ��������� ����� � ���������� ����!
		m_isRun = true;
		std::thread t1([this]()->void {
			if (m_init)
				while (m_frame());
		});
		while (m_isRun)
			Window::Get()->RunEvent();
		t1.join();
	}	

	bool Framework::m_frame()
	{
		/*/ ������������ ������� ����
		m_wnd->RunEvent();*/
		if (!m_isRun)
		{
			m_isRun = false;
			return false;
		}
		// ���� ���� ��������� - ��������� ����
		if (!m_wnd->IsActive())
			return true;

		// ���� ���� ���� �������, ��������� ������ ������
		if (m_wnd->IsExit())
		{
			m_isRun = false;
			return false;
		}

		// ���� ���� �������� ������
		if (Window::Get()->IsResize())
		{
			m_render->m_resize();
		}

		m_render->BeginFrame();
		if (!m_render->Draw())
		{
			m_isRun = false;
			return false;
		}
		m_render->EndFrame();

		return true;
	}

	void Framework::Close()
	{
		m_init = false;
		m_render->Shutdown();
		_DELETE(m_render);
		_CLOSE(m_wnd);
		_CLOSE(m_input);
	}

//------------------------------------------------------------------
}