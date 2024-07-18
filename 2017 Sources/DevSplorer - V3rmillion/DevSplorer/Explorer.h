#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string> //for receiving and writting strings
#include <vector>
#include <iostream>
#include <TlHelp32.h>
#include <Psapi.h>
#include <msclr\marshal_cppstd.h>
#include "gameproperties.h"
#include <Winuser.h>

using namespace std;

namespace DevSplorer {

	using namespace System;
	using namespace System::Reflection;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Summary for Explorer
	/// </summary>
	public ref class Explorer : public System::Windows::Forms::Form
	{
	public:
		Explorer(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Explorer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TreeView^  explorerRewrite;
	protected:
	private: System::Windows::Forms::PropertyGrid^  properties;
	private: System::Windows::Forms::Label^  cStack;




	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->explorerRewrite = (gcnew System::Windows::Forms::TreeView());
			this->properties = (gcnew System::Windows::Forms::PropertyGrid());
			this->cStack = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// explorerRewrite
			// 
			this->explorerRewrite->BackColor = System::Drawing::Color::White;
			this->explorerRewrite->Font = (gcnew System::Drawing::Font(L"Courier New", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->explorerRewrite->Location = System::Drawing::Point(-1, 21);
			this->explorerRewrite->Name = L"explorerRewrite";
			this->explorerRewrite->Size = System::Drawing::Size(190, 218);
			this->explorerRewrite->TabIndex = 5;
			this->explorerRewrite->DoubleClick += gcnew System::EventHandler(this, &Explorer::explorerRewrite_DoubleClick);
			this->explorerRewrite->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Explorer::explorerRewrite_MouseDown);
			// 
			// properties
			// 
			this->properties->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->properties->HelpVisible = false;
			this->properties->Location = System::Drawing::Point(187, 21);
			this->properties->Name = L"properties";
			this->properties->Size = System::Drawing::Size(215, 218);
			this->properties->TabIndex = 6;
			this->properties->ToolbarVisible = false;
			this->properties->Click += gcnew System::EventHandler(this, &Explorer::properties_Click);
			// 
			// cStack
			// 
			this->cStack->AutoSize = true;
			this->cStack->BackColor = System::Drawing::Color::Transparent;
			this->cStack->Font = (gcnew System::Drawing::Font(L"Determination Mono", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->cStack->Location = System::Drawing::Point(3, 1);
			this->cStack->Name = L"cStack";
			this->cStack->Size = System::Drawing::Size(107, 18);
			this->cStack->TabIndex = 7;
			this->cStack->Text = L"Clear Stack";
			this->cStack->Click += gcnew System::EventHandler(this, &Explorer::cStack_Click);
			// 
			// Explorer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(399, 236);
			this->ControlBox = false;
			this->Controls->Add(this->cStack);
			this->Controls->Add(this->properties);
			this->Controls->Add(this->explorerRewrite);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Name = L"Explorer";
			this->Text = L"RTF\'s Explorer";
			this->Load += gcnew System::EventHandler(this, &Explorer::Explorer_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Explorer_Load(System::Object^  sender, System::EventArgs^  e) {

		DWORD vftable = explorerLib::SCVTable;

		explorerLib::SCV = Memory::mScan(PAGE_READWRITE, (char*)&vftable, "xxxx");
		//MessageBoxA(0, "test", "e", MB_OK);
		explorerLib::RState = explorerLib::fetchRState((DWORD*)explorerLib::SCV);
		//MessageBoxA(0, "test", "e", MB_OK);

		explorerLib::DataModel = explorerLib::GetParent(explorerLib::SCV);

		//MessageBoxA(0, "test", "e", MB_OK);

		vector<DWORD> explorerinit = explorerLib::GetChildren(explorerLib::DataModel);

		for (int i = 0; i < explorerinit.size(); ++i) {
			string lie = *explorerLib::GetName(explorerinit[i]);
			explorerRewrite->Nodes->Add((msclr::interop::marshal_as<String^>(*explorerLib::GetName(explorerinit[i]))));
		}

		//MessageBoxA(0, "test", "e", MB_OK);

		Memory_Watch();
		MemoryWatch_Start();
		GetCertainProperty();
		r_getfield_explorer(explorerLib::RState, -10002, "game");
		MemoryWatch_End();

		GLobalXPlorer^ xplorers = gcnew GLobalXPlorer;
		xplorers->globalproperties = properties;
		xplorers->globalxplorer = explorerRewrite;

	}

	private: System::Void properties_Click(System::Object^  sender, System::EventArgs^  e) {

	}

	private: System::Void explorerRewrite_DoubleClick(System::Object^  sender, System::EventArgs^  e) {

		GLobalXPlorer^ xplorers = gcnew GLobalXPlorer;
		xplorers->globalproperties = properties;
		xplorers->globalxplorer = explorerRewrite;

		MemoryWatch_Start();
		IndexExplorer(explorerRewrite);

		RProperty^ explorerProperties = gcnew RProperty;
		properties->SelectedObject = explorerProperties;
		properties->Refresh();

		for (const string &property : GlobalProperties)
		{
			if (property != "")
			{
				FetchAllProperties(property.c_str());
			}
		}

		properties->Refresh();
		MemoryWatch_End();


	}
private: System::Void clearStackToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {


	
}

private: System::Void cStack_Click(System::Object^  sender, System::EventArgs^  e) {

	if (ExplorerIndex::ClearingStack == true)
	{
		ExplorerIndex::ClearingStack = false;
		properties->SelectedObject = nullptr;
		properties->Refresh();

		EmptyStack(explorerLib::RState);
		MemoryWatch_Start();
		r_emptystack(explorerLib::RState);
		MemoryWatch_End();

		explorerRewrite->Nodes->Clear();
		ExplorerIndex::IndexedBefore = false;
		ExplorerIndex::CurrentIndex = NULL;

		IndexStack.clear();

		vector<DWORD> explorerinit = explorerLib::GetChildren(explorerLib::DataModel);

		for (int i = 0; i < explorerinit.size(); ++i) {
			string lie = *explorerLib::GetName(explorerinit[i]);
			explorerRewrite->Nodes->Add((msclr::interop::marshal_as<String^>(*explorerLib::GetName(explorerinit[i]))));
		}

		MemoryWatch_Start();
		r_getfield_explorer(explorerLib::RState, -10002, "game");
		MemoryWatch_End();		
		ExplorerIndex::ClearingStack = true;
	}
}
private: System::Void explorerRewrite_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

	if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		System::Windows::Forms::ContextMenu ^cm = gcnew System::Windows::Forms::ContextMenu();
		if (explorerRewrite->SelectedNode)
		{
			DisplayRightMenu(this, cm, e, explorerRewrite);
		}
	}
}
};
}
