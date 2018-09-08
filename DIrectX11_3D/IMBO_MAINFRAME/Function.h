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
	//wchar_t* t = new wchar_t[len]; //�����Ҵ� ���� ����
	//if (NULL == t) throw std::bad_alloc();
	mbstowcs(r_val, all, len);
	//return (TCHAR*)t;
}

class CTagFinder	//�������̳ʿ��� find_if�� ���ϴ� ��ũ�� ���� ã���� ���
{
public:
	explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
	~CTagFinder(void) {}
public:
	template <typename T> bool operator () (T& Pair)
	{
		int iResult = lstrcmp(m_pTag, Pair.first); //lstrcmp �Լ��� �����ڵ�, ��Ƽ����Ʈ�� ������� �ʰ� ���ڿ��� ���Ѵ�.

		if (0 == iResult) //���� ���� 0�̸� �� ���ڿ��� ������ �����̸� ���� ���ڿ��� �� �������̰� ����̸� ���� ���ڿ��� �� ū ���ڿ� �̴�.
			return true;

		return false;
	}
private:
	const TCHAR*	m_pTag;	//�Լ� ��ü�� �����ڷ� �����Ҷ� ���� ���ڰ����� �ʱ�ȭ�Ͽ� () �Լ� ȣ���� ���� ���ϱ� ���ؼ� ����
};