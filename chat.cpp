#include <iostream>
#include <thread>
#include <fstream>
#include <semaphore>
#include <fstream>

using namespace std;

char fileName[] = "buffer_messenger.txt";

void write_file(std::fstream &buff, const std::string &name, const std::binary_semaphore &sem)
{
    buff << name << ":";
    buff.write((char *)&sem, sizeof(sem));
    buff << "#";
}

void parsing(std::fstream &buff, std::string &name)
{
    while (buff.peek() != ':')
    {
        name.push_back(buff.get());
    }
    buff.get();
}

bool try_read(std::fstream &buff, const std::string &name, std::binary_semaphore &sem)
{
    while (buff.peek() != EOF)
    {
        std::string tmp;
        parsing(buff, tmp);

        if (tmp == name)
        { // read semaphore.
            buff.read((char *)&sem, sizeof(sem));
            return true;
        }
        else
        {
            while (!buff.eof() && buff.peek() != '#')
            {
                buff.ignore();
            }
            if (!buff.eof())
            {
                buff.ignore();
            }
        }
    }
    return false;
}

void rewrite(const std::string &name, std::binary_semaphore &sem)
{
    std::fstream buff("f.txt", std::ios::in | std::ios::out | std::ios::binary);
    int indent = 0;
    while (true)
    {
        std::string tmp;
        parsing(buff, tmp);
        indent += tmp.size();
        ++indent;
        if (tmp == name)
        {
            // std::cout << "rewrite " << tmp << std::endl;
            buff.seekp(indent);
            buff.write((char *)&sem, sizeof(sem));
            break;
        }

        while (buff.peek() != '#')
        {
            indent += 1;
            buff.ignore();
        }
        buff.ignore();
        indent += 1;
    }
    buff.close();
}

void wait(const std::string &name, std::binary_semaphore &sem)
{
    std::fstream buff("f.txt", std::ios::in | std::ios::out | std::ios::binary);
    int indent = 0;
    while (true)
    {
        std::string tmp;
        parsing(buff, tmp);
        indent += (tmp.size() + 1);
        if (tmp == name)
        {
            while (true)
            {
                std::string tmp;

                int i;
                for (i = 0; buff.peek() == '\0'; ++i)
                {

                    char ch = buff.get();
                    tmp.push_back(ch);
                }
                bool flag = true;
                if (i != 8) // вывод на экран
                {
                    for (int i = 0; buff.peek() != ':'; ++i)
                        buff.seekg(indent - i);
                    buff.get();
                    std::binary_semaphore s{0};
                    buff.write((char *)&s, sizeof(s));
                    buff.close();
                    return;
                }

                for (int i = 0; buff.peek() != ':'; ++i)
                    buff.seekg(indent - i);
                buff.get();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            std::cout << "+++++\n";

            break;
        }

        while (buff.peek() != '#')
        {
            indent += 1;
            buff.ignore();
        }
        buff.ignore();
        indent += 1;
    }
    buff.close();
}

void open_semaphore(const std::string &file_name, const std::string &name_from, std::binary_semaphore &sem)
{
    std::fstream file(file_name.c_str(), std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    std::fstream buff_1(file_name.c_str(), std::ios::in | std::ios::out | std::ios::app | std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "err\n";
        return;
    }
    bool flag_write = false;
    if (!try_read(file, name_from, sem))
    {
        flag_write = true;
    }

    if (flag_write)
    {
        write_file(buff_1, name_from, sem);
    }
    file.close();
    buff_1.close();
}

void ReadProcess(const char *from, const char *to)
{
    std::binary_semaphore sem2{0};
    open_semaphore("f.txt", to, sem2);
    while (true)
    {
        wait(to, sem2);
        fstream file(fileName, fstream::in);

        while (!file.eof())
        {
            char buf[100];
            file.getline(buf, sizeof(buf));
            cout << buf << std::endl;
        }
    }
}

void WriteProcess(const std::string &from, const std::string &to)
{
    std::binary_semaphore sem1{0};
    open_semaphore("f.txt", from, sem1);
    while (true)
    {
        string t;
        cin >> t;
        fstream file(fileName, fstream::out); // записать в файл
        file << from << ": " << t;
        file.close();
        sem1.release();
        rewrite(from, sem1);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "input args" << std::endl;
        return 0;
    }

    thread t1(ReadProcess, argv[1], argv[2]);
    thread t2(WriteProcess, argv[1], argv[2]);
    t1.join();
    t2.join();

    std::remove("f.txt");

    return 0;
}