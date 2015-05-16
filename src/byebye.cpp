/*
The MIT License (MIT)

Copyright (c) 2015, Alexis Nunes

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <nan.h>
#include <windows.h>

using namespace v8;

typedef struct
{
	int pid;
	bool done;
} PARAMS;

static BOOL CALLBACK ScanAllWindows( HWND hWnd, LPARAM lParam) {
	DWORD dwProcessId;
	PARAMS *data = reinterpret_cast<PARAMS *>(lParam);

	GetWindowThreadProcessId( hWnd, &dwProcessId);
	if( data->pid == dwProcessId ) {
		if( GetWindowLong( hWnd, GW_OWNER ) == NULL ) {
			if( SendNotifyMessage(hWnd, WM_CLOSE, 0, 0) ) {
				data->done = true;
			}
		}
	}
	return TRUE;
}

NAN_METHOD(CloseMainWindowsByProcess) {
	PARAMS data;
	NanScope();

	if( args.Length() != 1 ) {
		NanReturnUndefined();
	}
	if( ! ( args[0]->IsObject() ) ) {
		NanReturnUndefined();
	}
	Local<Object> obj = args[0].As<Object>();
	Local<Value> pidValue = obj->Get( NanNew<String>( "pid" ) );
	if( ! ( pidValue->IsInt32() ) ) {
		NanReturnUndefined();
	}
	data.pid = pidValue->Int32Value();
	data.done = false;

	EnumWindows( ScanAllWindows, reinterpret_cast<LPARAM>(&data) );
	NanReturnValue( NanNew<Boolean>( data.done ) );
}

void Init(Handle<Object> exports) {
	exports->Set( NanNew<String>( "CloseMainWindowsByProcess" ), NanNew<FunctionTemplate>( CloseMainWindowsByProcess )->GetFunction());
}

NODE_MODULE(byebye, Init)
