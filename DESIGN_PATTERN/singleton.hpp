class Singleton
{
    private static Singleton m_instance;
    private Singleton()
    {
        ...
    }

    public static synchronized Singleton getInstance()
    {
        if (m_instance == null)
            m_instance = new Singleton();

        return m_instance;
    }
    ...
    public void doSomething()
    {
        ... 
    }
}
