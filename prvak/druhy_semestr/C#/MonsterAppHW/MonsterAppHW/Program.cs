namespace MonsterAppHW;

class Program
{
    static void Main(string[] args)
    {
        // set how many repetitions or monster should be in app
        int repetitonsCount = 20;
        int monsterCount = 2;

        MonsterApp app = new MonsterApp();
        app.RunApp(repetitonsCount, monsterCount);
    }
}