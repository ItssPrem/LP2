import streamlit as st
from nltk.chat.util import Chat, reflections

# Movie data
movies = {
    'Interstellar': ['10:00 AM', '2:00 PM', '6:00 PM'],
    'The Dark Knight': ['11:00 AM', '3:00 PM', '7:00 PM'],
    'Dune': ['9:30 AM', '1:30 PM', '5:30 PM']
}
ticket_price = 100

# Chatbot patterns
patterns = [
    (r'hi|hello|hey', ['👋 Hello! I can help you book movie tickets.']),
    (r'(.*)(thank you|thanks|thank|thx|appreciate)', ["🎉 You're welcome! Enjoy your movie! 🍿"]),
    (r'(.*) movie|movies|show|shows', [f"🎬 Movies now playing: {', '.join(movies.keys())}"]),
    (r'(.*) showtimes?|times?|slots?', [f"🕒 Showtimes: " + ' | '.join(f"{m}: {', '.join(t)}" for m, t in movies.items())]),
    (r'(.*) book tickets|book a ticket', ["🎟 Sure! Which movie would you like to watch?"]),
    (r'(.*) cancel ticket|cancel booking', ["❌ Booking canceled."]),
    (r'(.*) contact|email|phone|help', ["📧 Email: support@movietickets.com | 📞 Phone: +1234567890"]),
    (r'(.*)(bye|exit|goodbye)', ["👋 Goodbye! Enjoy your movie!"]),
    (r'(.*)', ["🤖 I didn't understand. Try 'show movies', 'book tickets', or 'showtimes'."]),
]
bot = Chat(patterns, reflections)

# Streamlit setup
st.set_page_config(page_title="🎟 Movie Ticket Bot")
st.title("🎟 Movie Ticket Booking Bot")

# Suggestion
st.markdown("💡 *Try:* show movies, book tickets, showtimes, cancel ticket, contact")

# Session state
if "chat" not in st.session_state: st.session_state.chat = []
if "step" not in st.session_state: st.session_state.step = None
if "data" not in st.session_state: st.session_state.data = {}

# Display chat
for who, msg in st.session_state.chat:
    st.markdown(f"<div style='text-align:{'right' if who=='You' else 'left'}; margin:6px 0;'><b>{who}:</b> {msg}</div>", unsafe_allow_html=True)

# Booking steps
def booking_flow(msg):
    step, data = st.session_state.step, st.session_state.data
    if step == "movie":
        movie = msg.title()
        if movie in movies:
            data["movie"] = movie
            st.session_state.step = "tickets"
            return f"🎟 How many tickets for {movie}? (₹{ticket_price} each)"
        return f"❌ Movie not found. Try: {', '.join(movies.keys())}"
    if step == "tickets":
        if msg.isdigit() and int(msg) > 0:
            data["tickets"] = int(msg)
            st.session_state.step = "showtime"
            return f"🕒 Choose a showtime for {data['movie']}: {', '.join(movies[data['movie']])}"
        return "❌ Enter a valid number."
    if step == "showtime":
        if msg in movies[data["movie"]]:
            total = data["tickets"] * ticket_price
            st.session_state.step, st.session_state.data = None, {}
            return f"✅ Booked! 🎬 {data['movie']} at {msg}\n🎟 {data['tickets']} ticket(s) | 💵 ₹{total}\nEnjoy your movie! 🍿"
        return f"❌ Invalid time. Options: {', '.join(movies[data['movie']])}"

# Input form
with st.form("chat_form", clear_on_submit=True):
    user_input = st.text_input("Type your message...", placeholder="e.g., Book tickets")
    if st.form_submit_button("Send") and user_input:
        st.session_state.chat.append(("You", user_input))
        msg = user_input.lower()

        # Handle exit conditions first
        if any(x in msg for x in ["bye", "exit", "goodbye"]):
            reply = "👋 Goodbye! Enjoy your movie!"
            st.session_state.step, st.session_state.data = None, {}
        
        # Handle movie selection and showtimes
        elif "movie" in msg or "showtimes" in msg or "movies" in msg:
            reply = f"🎬 Movies now playing: {', '.join(movies.keys())}"
        
        # Handle booking process
        elif "book" in msg:
            st.session_state.step, st.session_state.data = "movie", {}
            reply = f"🎟 Sure! Which movie would you like to watch?  \nAvailable movies: {', '.join(movies.keys())}"
        
        # Handle cancel booking
        elif "cancel" in msg:
            st.session_state.step, st.session_state.data = None, {}
            reply = "❌ Booking canceled. Let me know if you want to book again!"
        
        # Handle contact information
        elif "contact" in msg or "email" in msg or "phone" in msg:
            reply = "📧 Email: support@movietickets.com | 📞 Phone: +1234567890"
        
        # Execute the booking flow if in booking state
        elif st.session_state.step:
            reply = booking_flow(user_input)
        
        # Fallback for other cases
        else:
            reply = bot.respond(user_input)

        st.session_state.chat.append(("Bot", reply))
        st.rerun()

# Reset button
st.divider()
if st.button("🗑 Reset Chat"):
    st.session_state.chat, st.session_state.step, st.session_state.data = [], None, {}
    st.rerun()