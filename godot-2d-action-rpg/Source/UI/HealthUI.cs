using Godot;
using System;

public class HealthUI : Control
{
    private TextureRect _heartUiFull;
    private TextureRect _heartUiEmpty;
    private Stats       _playerStats;

    private int         _hearts;
    private int         _maxHearts;

    [Export]
    public int MaxHearts
    {
        get => _maxHearts;
        set
        {
            _maxHearts = Mathf.Max(value, 1);
            this._hearts = Mathf.Min(_hearts, _maxHearts);
            if (_heartUiEmpty != null)
            {
                _heartUiEmpty.RectSize= new Vector2(_maxHearts * 15.0f, 11.0f);
            }
        }
    }

    [Export]
    public int Hearts
    {
        get => _hearts;
        set
        {
            _hearts = Mathf.Clamp(value, 0, _maxHearts);
            // _hearts = value;
            if (_heartUiFull != null)
            {
                _heartUiFull.RectSize= new Vector2(_hearts * 15.0f, 11.0f);
            }
        }
    }

    public override void _Ready()
    {
        _heartUiFull  = GetNode<TextureRect>("HeartUIFull");
        _heartUiEmpty = GetNode<TextureRect>("HeartUIEmpty");

        _playerStats = GetNode("/root/PlayerStats") as Stats;
        if (_playerStats != null)
        {
            _setMaxHearts(_playerStats.MaxHealth);
            _playerStats.Connect("MaxHealthChanged", this, "_setMaxHearts");

            _setHearts(_playerStats.Health);
            _playerStats.Connect("HealthChanged", this, "_setHearts");
        }
    }

    public override void _Process(float delta)
    {}

    private void _setMaxHearts(int value)
    {
        MaxHearts = value;
    }

    private void _setHearts(int value)
    {
        Hearts = value;
    }

}
