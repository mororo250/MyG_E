#include "FileBrowser.h"


HRESULT create_file_dialog(IFileDialog** file_dialog, CLSID dialog_type)
{
	// initialize the COM library.
	HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(result))
	{
		// Create the FileOpenDialog object.
		result = CoCreateInstance(dialog_type, NULL,
			CLSCTX_INPROC_SERVER, IID_PPV_ARGS(file_dialog));
	}
	return result;
}

bool show_file_dialog(IFileDialog* file_dialog, std::string& filename)
{
	// Show the Open dialog box.
	HRESULT result = file_dialog->Show(NULL);

	// Get the file name from the dialog box.
	if (SUCCEEDED(result))
	{
		IShellItem* item;
		result = file_dialog->GetResult(&item);
		if (SUCCEEDED(result))
		{
			PWSTR file_path;
			result = item->GetDisplayName(SIGDN_FILESYSPATH, &file_path);

			// Display the file name to the user.
			if (SUCCEEDED(result))
			{
				//MessageBox(NULL, file_path, L"File Path", MB_OK);
				CoTaskMemFree(file_path);
			}
			else
				return false;

			// convert to string
			std::wstring aux(file_path);
			filename.assign(aux.begin(), aux.end());

			item->Release();
		}
		else
			return false;

		return true;
	}
	else
		return false;
}

std::string open_file_browser(std::wstring const& file_types, std::wstring const& filter_name)
{
	std::string filename = "";

	IFileDialog* file_dialog = nullptr;
	HRESULT result = create_file_dialog(&file_dialog, CLSID_FileOpenDialog);
			
	if (SUCCEEDED(result))
	{
		// Set file types.
		COMDLG_FILTERSPEC filter_spec[] =
		{
			{ file_types.c_str(), filter_name.c_str() }
		};
		file_dialog->SetFileTypes(1, filter_spec);

		show_file_dialog(file_dialog, filename);
		
		file_dialog->Release();
	}
	CoUninitialize();

	return filename;
}

std::string save_file_browser(std::wstring const& file_types, std::wstring const& filter_name)
{
	std::string filename;

	IFileDialog* file_dialog = nullptr;
	HRESULT result = create_file_dialog(&file_dialog, CLSID_FileSaveDialog);

	if (SUCCEEDED(result))
	{
		// Set file types.
		COMDLG_FILTERSPEC filter_spec[] =
		{
			{ file_types.c_str(), filter_name.c_str() }
		};
		file_dialog->SetFileTypes(1, filter_spec);

		show_file_dialog(file_dialog, filename);
		
		file_dialog->Release();
	}
	CoUninitialize();

	return filename;
}

std::string open_folder_browser()
{
	std::string folder_name;

	IFileDialog* file_dialog = nullptr;
	HRESULT result = create_file_dialog(&file_dialog, CLSID_FileOpenDialog);

	if (SUCCEEDED(result))
	{
		DWORD dwOptions;
		if (SUCCEEDED(file_dialog->GetOptions(&dwOptions)))
			file_dialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

		show_file_dialog(file_dialog, folder_name);

		file_dialog->Release();
	}
	CoUninitialize();

	return folder_name;
}