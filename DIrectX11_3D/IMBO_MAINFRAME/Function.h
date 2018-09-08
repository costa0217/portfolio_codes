#pragma once

template<typename Ty, size_t N>
constexpr size_t GetArraySize(Ty(&)[N]) noexcept
{
	return N;
}

template <typename T> void Safe_EndDelete(T& pointer)
{
	if (pointer)
	{
		pointer->End();
		delete pointer;
		pointer = nullptr;
	}
}
template <typename T> void Safe_Delete(T& pointer)
{
	if (pointer)
	{
		delete pointer;
		pointer = nullptr;
	}
}

static std::string GetFileName(std::string path) {
	size_t size = path.size();
	char name[64];
	int index{ 0 };
	for (size_t i = size - 1; i > 1; i--) {
		if (path[i] == '\\' | path[i] == '/') {
			for (size_t j = i + 1; j < size; ++j) {
				if (path[j] == '.') break;
				name[index++] = path[j];
			}
			name[index] = '\0';
			break;
		}
	}
	return name;
}

static std::string TCHARToString(const TCHAR* ptsz)
{
	size_t len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return s;
}

static void StringToTCHAR(std::string& s, TCHAR* r_val)
{
	std::string tstr;
	const char* all = s.c_str();
	size_t len = 1 + strlen(all);
	//wchar_t t[256];
	//wchar_t* t = new wchar_t[len]; //동적할당 하지 않음
	//if (NULL == t) throw std::bad_alloc();
	mbstowcs(r_val, all, len);
	//return (TCHAR*)t;
}

class CTagFinder	//맵컨테이너에서 find_if로 원하는 태크의 맵을 찾을때 사용
{
public:
	explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
	~CTagFinder(void) {}
public:
	template <typename T> bool operator () (T& Pair)
	{
		int iResult = lstrcmp(m_pTag, Pair.first); //lstrcmp 함수는 유니코드, 멀티바이트를 고려하지 않고 문자열을 비교한다.

		if (0 == iResult) //리턴 값이 0이면 두 문자열은 같으며 음수이면 왼쪽 문자열이 더 작은것이고 양수이면 왼쪽 문자열이 더 큰 문자열 이다.
			return true;

		return false;
	}
private:
	const TCHAR*	m_pTag;	//함수 객체를 생성자로 생성할때 받은 인자값으로 초기화하여 () 함수 호출을 통해 비교하기 위해서 선언
};