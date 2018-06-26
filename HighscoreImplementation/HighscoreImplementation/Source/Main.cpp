#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <ScoreSystem.h>
#include <string>
#include <algorithm>


void DrawList(nana::listbox& list, scoreSystem::ScoreSystem& scoreTable);

int main()
{
	using namespace nana;
	scoreSystem::ScoreSystem scoreTable(10);

	//Define a form.
	form fm;
	fm.caption("Highscore");
	fm.size(size(400, 600));

	//Define a label and display a text.
	label lab{ fm, "New Score:"};
	lab.format(true);

	textbox tb{ fm };
	spinbox sb{ fm };

	listbox list{ fm,true };
	list.append_header("Nombre");
	list.append_header("Score");
	DrawList(list, scoreTable);
	button btnAdd{ fm, "Add" };
	btnAdd.events().click([&fm, &scoreTable, &list, &tb, &sb] {
		std::string name;
		tb.getline(0, name);
		name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
		if (name != "") {
			scoreTable.AddScore(name.c_str(), std::stoi(sb.value()));
		}
		DrawList(list, scoreTable);
	});
	button btnRemove{ fm, "Remove" };
	btnRemove.events().click([&fm,&scoreTable, &list] {
		listbox::index_pairs selected = list.selected();
		for (unsigned int i = 0; i < selected.size(); i++) {
			scoreTable.RemoveScore(selected[i].item);
		}
		DrawList(list, scoreTable);
	});
	//Define a button and answer the click event.
	button btnQuit{ fm, "Quit" };
	btnQuit.events().click([&fm] {
		fm.close();
	});

	//Layout management
	fm.div("vert <weight=80% listbox><<weight=20% text><tb><sb>><><weight=24<buttonQ><buttonR><buttonA>><>");
	fm["text"] << lab;
	fm["tb"] << tb;
	fm["sb"] << sb;
	fm["buttonA"] << btnAdd;
	fm["buttonR"] << btnRemove;
	fm["buttonQ"] << btnQuit;
	fm["listbox"] << list;
	fm.collocate();

	//Show the form
	fm.show();

	//Start to event loop process, it blocks until the form is closed.
	exec();
}

void DrawList(nana::listbox& list, scoreSystem::ScoreSystem& scoreTable) {
	auto cat = list.at(0);
	for (unsigned int i = 0; i < scoreTable.GetSize(); i++) {
		cat.push_back({});
		std::string scoreName = scoreTable.GetScore(i)._name;
		std::string scoreText = std::to_string(scoreTable.GetScore(i)._value);
		list.at(nana::listbox::index_pair(0, i)).text(0, scoreName);
		list.at(nana::listbox::index_pair(0, i)).text(1, scoreText);
	}
}